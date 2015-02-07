// translator.cpp
#include "translator.h"
#include <fstream>
#include <sstream>
//====================================================
//= struct translator
//====================================================
translator::translator(std::ostream &os, const fdg::navigater &navigater)
: os(os)
, navigater(navigater)
{ }
translator &translator::operator << (const string &ebc)
{
	translate(ebc);
	return *this;
}
void translator::translate(const string &ebc)
{
	void (translator::*func)(std::istream &) = &translator::translate;
	if (path::file_exists(ebc.sjis().c_str()))
	{
		std::ifstream ifs(ebc.sjis().c_str());
		(this->*func)(ifs);
	}
	else 
	{
		std::istringstream iss(ebc.c_str());
		(this->*func)(iss);
	}
}
void translator::translate(std::istream &is)
{
	//JSON追記（配列の始まり）
	os << "[\n";

	//レコード長分のバッファ
	string buf(navigater.rsize, 0);

	//処理行数カウント
	struct { int lines; } done;
	done.lines = 0;

	while (is && is.read(&buf[0], buf.size()))
	{
		//JSON追記（オブジェクトの始まり）
		os << "\t";
		os << (done.lines++ ? ", " : "  ");
		os << "{\n";

		//ナビゲータループ
		for (fdg::navigater::const_iterator b = navigater.begin(), i = b, e = navigater.end()
			; i != e; ++i)
		{
			//JSON追記（キーと値のペアの始まり）
			os << "\t\t";
			os << (i - b ? ", " : "  ");

			//ナビゲータによる変換
			const fdg::field &field = *i;
			int		real	= field.real	;
			int		offset	= field.offset	;
			int		left	= field.left	;
			int		right	= field.right	;
			bool	pack	= field.pack	;
			bool	sig		= field.sig		;
			string	type	= field.type	;
			string	key		= field.key		;

			//変換対象EBCDIC文字列切り出し
			string ebc = buf.substr(offset, real);

			//変換後SJIS文字列格納用
			string sjis;

			//========================================
			//= タイプ別変換処理
			//========================================
			bool kanji = (type == "N");
			if (kanji)
			{
				//====================================
				//= JEF漢字
				//====================================
				for (string::iterator i = ebc.begin(), e = ebc.end()
					; i != e; ++i)
				{
					BYTE hi = (BYTE)*i;
					BYTE lo = (BYTE)*(++i);
					WORD jef = MAKEWORD(lo, hi);
					WORD conv = ebcdic::jef::tosjis(jef);
					sjis += (char)HIBYTE(conv);
					sjis += (char)LOBYTE(conv);
				}
			}
			else if (pack)
			{
				//====================================
				//= パック項目
				//====================================
				for (string::iterator i = ebc.begin(), e = ebc.end()
					; i != e; ++i)
				{
					//16進表記に変換
					sjis += string::format("%02x", (uchar)(*i) & 0xff);
				}
			}
			else 
			{
				//====================================
				//= 半角英数 (PIC 9 or X)
				//====================================
				for (string::iterator i = ebc.begin(), e = ebc.end()
					; i != e; ++i)
				{
					sjis += (char)ebcdic::tosjis((BYTE)(*i));
				}
			}

			if (pack || sig)
			{
				//====================================
				//= パック項目、符号付き項目
				//====================================
				//末尾1文字ポップ
				string tail = sjis.pop(-1);
				//パックの場合、桁数を調整
				if (pack && (int)sjis.length() > (left + right))
				{
					sjis = sjis.substr(sjis.length() - (left + right));
				}

				if (pack)
				{
					//末尾の1文字で符号判定
					if (tail == "D") sjis = "-" + sjis;
				}
				else 
				{
					//符号付き項目の判定
					// A : +0   J : -0
					// B : +1   K : -1
					// C : +2   L : -2
					//   ...      ...
					// I : +9   R : -9
					int def = tail[0] - 'A';
					if (def > 10) sjis = "-" + sjis;
					sjis += string::format("%d", def % 10);
				}
			}

			if (right)
			{
				//====================================
				//= 小数点
				//====================================
				string dec = sjis.pop(-right);
				sjis += ".";
				sjis += dec;
			}



			//JSONに追記（キーと値）
			os << json_escape(key.utf8()).double_quote();
			os << ": ";
			os << json_escape(sjis.utf8()).double_quote();
			os << "\n";


		}

		//JSON追記（オブジェクトの終わり）
		os << "}\n";
	}

	//JSON追記（配列の終わり）
	os << "]";
}
//====================================================
//JSONエスケープ
//====================================================
string translator::json_escape(const string &s)
{
	string r;
	for (string::const_iterator i = s.begin(), e = s.end()
		; i != e; ++i)
	{
		uchar c = (uchar)*i;
		switch (c)
		{
		case '"'	: r += "\\\""	; break;
		case '\\'	: r += "\\\\"	; break;
		case '/' 	: r += "\\/"	; break;
		default		: r += c;
		}
	}
	return r;
}
//====================================================
//====================================================
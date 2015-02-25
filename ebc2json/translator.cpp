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
			string	key		= field.key		;
			
			//変換対象EBCDIC文字列切り出し
			string ebc = buf.substr(offset, real);
			
			//変換後SJIS文字列格納用
			string sjis;

			//EBCDIC => SJIS変換
			field.translate(ebc, sjis);//★

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
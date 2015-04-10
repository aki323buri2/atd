// fdg.cpp
#include "fdg.h"
#include <fstream>
#include <sstream>
#include "atd.regex.h"
#include <set>
#include "ebcdic.h"
//====================================================
//= strut fdg::field
//====================================================
string fdg::search(const string &ebc)
{
	//EBCDICファイル名からFDGファイルパスを類推する
	string filename = path::find_file_name(ebc);
	string name = filename.until(".");
	string lib = filename;
	//サフィックス候補
	strings suffs;
	suffs.entry("");
	suffs.entry("." + lib);
	suffs.entry(".RDMLIB");
	//フォルダ候補
	strings dirs;
	dirs.entry(".");
	dirs.entry("..\\fdg");

	string s;
	for (strings::iterator di = dirs.begin(), de = dirs.end(); di != de; ++di)
	{
		string &dir = *di;
		for (strings::iterator si = suffs.begin(), se = suffs.end(); si != se; ++si)
		{
			string &suf = *si;
			string path = path::app_path(dir + "\\" + name + suf + ".FDG.txt");
			if (path::file_exists(path.sjis()))
			{
				s = path;
				break;
			}
		}
		if (s.length()) break;
	}
	return s;
}
//====================================================
//= struct fdg::field
//====================================================
fdg::field::field()
: lv(0)
, name("")
, sig(false)
, type("")
, left(0)
, pack(false)
, occurs(0)
, key("")
, sub(0)
, real(0)
, offset(0)
{ }
string fdg::field::translate(const string &ebc) const
{
	string sjis;
	translate(ebc, sjis);
	return sjis;
}
void fdg::field::translate(const string &ebc, string &sjis) const
{
	//タイプ別変換処理
	bool kanji = (type == "N");
	if (kanji)
	{
		//============================================
		//= JEF漢字
		//============================================
		for (string::const_iterator i = ebc.begin(), e = ebc.end()
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
		//============================================
		//= パック項目
		//============================================
		for (string::const_iterator i = ebc.begin(), e = ebc.end()
			; i != e; ++i)
		{
			//16進表記に変換
			sjis += string::format("%02x", (uchar)(*i) & 0xff);
		}
	}
	else
	{
		//============================================
		//= 半角英数 (PIC 9 or X)
		//============================================
		for (string::const_iterator i = ebc.begin(), e = ebc.end()
			; i != e; ++i)
		{
			sjis += (char)ebcdic::tosjis((BYTE)(*i));
		}
	}

	if (pack || sig)
	{
		//============================================
		//= パック項目、符号付き項目の末尾１バイト処理
		//============================================
		//末尾１文字ポップ
		string tail = sjis.pop(-1);
		//パックの場合、桁数を調整
		if (pack && (int)sjis.length() > (left + right))
		{
			sjis = sjis.substr(sjis.length() - (left + right));
		}
		//末尾処理
		if (pack)
		{
			//末尾の１文字で符号判定
			if (tail == "D") sjis = "-" + sjis;
		}
		else 
		{
			//符号付き項目の判定
			// '{' : +0  '}' : -0
			// 'A' : +1  'J' : -1
			// 'B' : +2  'K' : -2
			// 'C' : +3  'L' : -3
			//    ...       ...
			// 'I' : +9  'R' : -9
			uchar c = tail[0];
			bool minus = false;
			int n = 0;

			if (false) { }
			else if (c == '}') { n = 0; minus = false; }
			else if (c == '{') { n = 0; minus = true ; }
			else if (BETWEEN('A', c, 'I')) { n = c - 'A'; minus = false; }
			else if (BETWEEN('J', c, 'R')) { n = c - 'J'; minus = true ; }
			if (minus) sjis = "-" + sjis;
			sjis += string::format("%d", n);
		}
	}

	if (right)
	{
		//============================================
		//= 小数点
		//============================================
		string dec = sjis.pop(-right);
		sjis += ".";
		sjis += dec;
	}		
}
//====================================================
//= struct fdg::fields
//====================================================
fdg::fields::fields()
: offset(0)
{ }
fdg::fields &fdg::fields::operator << (const string &cobol)
{
	this->cobol(cobol);
	return *this;
}
void fdg::fields::cobol(const string &cobol)
{
	void (fdg::fields::*func)(std::istream &) = &fdg::fields::cobol;
	if (path::file_exists(cobol.sjis()))
	{
		std::ifstream ifs(cobol.sjis().c_str());
		(this->*func)(ifs);
	}
	else 
	{
		std::istringstream iss(cobol.c_str());
		(this->*func)(iss);
	}
}
void fdg::fields::cobol(std::istream &is)
{
	//正規表現（COBOL PICTURE句）
	string pattern = 
		"^([0-9]{6})?"	//行番号
		" *([0-9]{2})"	//LV
		" +([^ ]+)"	//識別名
		"("
			" +PIC +"
			"(S?)(9|X|N)"	//タイプ
			"\\(([0-9]+)\\)"	//桁数
			"(V9\\(([0-9]+)\\)|V(9+))?"	//小数点以下桁数
		")?"
		"( +(PACKED\\-DECIMAL|COMP\\-3))?"	//パック
		"( +OCCURS +([0-9]+))?"	//OCCURS
		;
	struct regex regex;
	regex.compile(pattern);

	//解析ループ
	string buf(0x100, 0);
	while (is && is.getline(&buf[0], buf.size()))
	{
		string subject = buf.c_str();
		strings match = regex.match(subject);
		bool success = match.size() > 1;
		if (success)
		{
			struct field field;
			field.lv	= match[ 2].toint();
			field.name	= match[ 3];
			field.sig	= match[ 5].length();
			field.type	= match[ 6];
			field.left	= match[ 7].toint();
		//	field.right	= match[ 9].toint();
			field.pack	= match[11].length();
			field.occurs= match[14].toint();//ずれてた！

			//小数点以下
			//'V9(NN)'パターンと'V99...'パターン
			string right = match[8];
			string nn = match[9];
			if (right.find('(') != string::npos)
			{
				//'V99...パターン'
				field.right = nn.length();
			}
			else if (right.find('V') == 0)
			{
				//なぜか[nn]に値が入らないので苦肉の策！★
				// field.right = nn.length();
				field.right = right.length() - 1;
			}
			else
			{
				//'V9(NN)'パターン
				field.right = nn.toint();
			}

			field.key = field.name;

			push_back(field);

		}
	}
}

//====================================================
//= strut fdg::navigater
//====================================================
fdg::navigater::navigater()
: rsize(0)
{ }
fdg::navigater &fdg::navigater::operator << (const struct fields &fields)
{
	import(fields);
	return *this;
}
void fdg::navigater::import(const struct fields &fields)
{
	std::set<string> checker;
	int offset = 0;
	import(checker, offset, fields.begin(), fields.end());
	rsize = offset;
}
fdg::navigater::const_iterator fdg::navigater::import(
	std::set<string> &checker
	, int &offset
	, const_iterator begin
	, const_iterator end
	, int sub//OCCURSインデックス
	)
{
	//インポート対象が空の場合に対応
	if (begin == end) return end;

	const struct field &field = *begin;
	int		lv		= field.lv		;
	string	name	= field.name	;
//	bool	sig		= field.sig		;
	string	type	= field.type	;
	int		left	= field.left	;
	int		right	= field.right	;
	bool	pack	= field.pack	;
	int		occurs	= field.occurs	;

	if (!occurs) occurs = 1;

	if (type == "")
	{
		//集団項目ならば再帰ループ
		++begin;
		const_iterator i;
		//OCCURS
		for (int o = 0; o < occurs; o++)
		{
			for (i = begin; i != end && i->lv > lv; )
			{
				i = import(checker, offset, i, end, o);
			}
		}
		return i;
	}
	//エントリ処理
	struct field add = field;
	//リアルサイズ計算
	int &real = add.real;
	real = left + right;
	if (type == "N")
	{
		//全角文字
		real *= 2;
	}
	if (pack)
	{
		//パック項目
		real = (real/2) + 1;
	}
	//OCCURS
	for (int o = 0; o < occurs; o++)
	{
		add.offset = offset;
		add.sub = sub;
		//名前の重複チェック
		string &name = add.name;
		string &key = add.key;
		key = name;
		int suffix = 0;
		while (checker.find(key) != checker.end())
		{
			key = string::format("%s_%d", name.c_str(), ++suffix);
		}
		checker.insert(key);
		push_back(add);
		offset += real;
	}

	return ++begin;//次のイテレータ
}
void fdg::navigater::demo(const generic::notify &notify) const 
{
	generic::notifyf notifyf = notify;

	//空の場合に対応
	if (size() == 0)
	{
		notify("fdg::navigater::demo() - 登録なし -");
		return;
	}
	//デモ開始
	for (const_iterator i = begin(), e = end(); i != e; ++i)
	{
		const struct field &field = *i;
		int		lv		= field.lv		;
		string	name	= field.name	;
		bool	sig		= field.sig		;
		string	type	= field.type	;
		int		left	= field.left	;
		int		right	= field.right	;
		bool	pack	= field.pack	;
	//	int		occurs	= field.occurs	;
		string	key		= field.key 	;
		int		real	= field.real	;
		int		offset	= field.offset	;
		int		sub		= field.sub		;

		notifyf("%d %-10s"
			" %4s"
			" %1s%1s"
			"(%2d)"
			"%-5s"
			" %-1s"
			" (%3d-%3d)"
			, lv, key.c_str()
			, (sub ? string::format("(%2d)", sub+1).c_str() : "")
			, (sig ? "S": ""), type.c_str()
			, left
			, (right ? string::format("V(%02d)", right).c_str() : "")
			, (pack ? "P" : "")
			, offset, real
		);
	}
}

//====================================================
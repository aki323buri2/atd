// fdg.cpp
#include "fdg.h"
#include <fstream>
#include <sstream>
#include "atd.regex.h"
#include <set>
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
//= strut fdg::field
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
//====================================================
//= strut fdg::fields
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
		"^ *([0-9]{2})"	//LV
		" +([^ ]+)"	//識別名
		"("
			" +PIC +"
			"(S?)(9|X|N)"	//タイプ
			"\\(([0-9]+)\\)"	//桁数
			"(V9\\(([0-9]+)\\))?"	//小数点以下桁数
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
			field.lv	= match[ 1].toint();
			field.name	= match[ 2];
			field.sig	= match[ 4].length();
			field.type	= match[ 5];
			field.left	= match[ 6].toint();
			field.right	= match[ 8].toint();
			field.pack	= match[10].length();
			field.occurs= match[12].toint();

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
	, int sub
	)
{
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
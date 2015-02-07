// atd.regex.cpp
#include "atd.regex.h"
#include "regex.h"
using namespace atd;
//====================================================
//= struct atd::regex::impl
//====================================================
struct regex::impl : public object 
{
	regex_t preg;
	int error;
	std::vector<regmatch_t> pmatch;
	impl()
	: error(0), pmatch(0x100, regmatch_t())
	{
		::memset(&preg, 0, sizeof(preg));
	}
	~impl()
	{
		clear();
	}
	void clear()
	{
		::regfree(&preg);
	}
	bool compile(const string &pattern)
	{
		error = ::regcomp(&preg, pattern.c_str(), REG_EXTENDED | REG_NEWLINE);
		return !error;
	}
	bool match(const string &subject, strings &match)
	{
		match.clear();
		if (error) return false;//コンパイルに失敗している

		int r = ::regexec(&preg, subject.c_str(), pmatch.size(), &pmatch[0], 0);
		if (r != 0) return false;

		//マッチしたグループの数
		int nsub = preg.re_nsub + 1;

		//マッチリスト作成
		for (int i = 0; i < nsub; i++)
		{
			string s;
			regmatch_t &regmatch = pmatch[i];
			int so = regmatch.rm_so;
			int eo = regmatch.rm_eo;
			if (so >= 0 && eo >= 0)
			{
				s = subject.substr(so, (eo - so));
			}
			match.entry(s);
		}

		return true;
	}

};
//====================================================
//= struct atd::regex
//====================================================
regex::regex()
: impl(new struct impl())
{ }
regex::~regex()
{
	delete impl;
}
bool regex::compile(const string &pattern)
{
	return impl->compile(pattern);
}
bool regex::match(const string &subject, strings &match)
{
	return impl->match(subject, match);
}
strings regex::match(const string &subject)
{
	strings match;
	this->match(subject, match);
	return match;
}
//====================================================
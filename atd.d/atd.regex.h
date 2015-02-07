// atd.regex.h
#ifndef __atd_regex_h__
#define __atd_regex_h__
#include "atd.h"
namespace atd {;
//====================================================
//= struct atd::regex
//====================================================
struct regex : public object 
{
	regex();
	~regex();
	bool compile(const string &pattern);
	bool match(const string &subject, strings &match);
	strings match(const string &subject);
private:
	struct impl;
	struct impl *impl;
	regex(const regex &);
};
//====================================================
}//namespace atd
#endif//__atd_regex_h__
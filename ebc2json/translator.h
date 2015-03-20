// translator.h
#ifndef __translator_h__
#define __translator_h__
#include "ebcdic.h"
#include "fdg.h"
//====================================================
//= struct translator
//====================================================
struct translator : public object
{
	std::ostream &os;
	const fdg::navigater &navigater;
	translator(std::ostream &os, const fdg::navigater &navigater);
	translator &operator << (const string &ebc);
	void translate(const string &ebc);
	void translate(std::istream &is);

	static string json_escape(const string &s);
};

#endif//__translator_h__
// ebcdic.h
#ifndef __ebcdic_h__
#define __ebcdic_h__
#include "atd.h"
using namespace atd;
//====================================================
//= struct ebcdic
//====================================================
struct ebcdic
{
	static BYTE EBC2SJIS[];
	static const char *JEF2JISEXSOURCE[];
	struct jef;

	static uchar tosjis(BYTE ebc);
};
//====================================================
//= struct ebcdic::jef
//====================================================
struct ebcdic::jef : public object, std::map<WORD, WORD>
{
	jef();
	static WORD tosjis(WORD def);
protected:
	WORD _tosjis(WORD jef) const;
	void expand();
};
//====================================================
#endif//__ebcdic_h__
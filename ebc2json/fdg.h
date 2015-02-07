// fdg.h
#ifndef __fdg_h__
#define __fdg_h__
#include "atd.h"
#include <set>
using namespace atd;
//====================================================
//= struct fdg
//====================================================
struct fdg
{
	struct field;
	struct fields;
	struct navigater;

	static string search(const string &ebc);
};
//====================================================
//= struct fdg::field
//====================================================
struct fdg::field : public object
{
	int		lv		;
	string	name	;
	bool	sig		;
	string	type	;
	int		left	;
	int		right	;
	bool	pack	;
	int		occurs	;

	string	key		;
	int		sub		;
	int		real	;
	int		offset	;

	field();
};
//====================================================
//= struct fdg::fields
//====================================================
struct fdg::fields : public object, public std::vector<field>
{
	int offset;
	fields();
	fields &operator << (const string &cobol);
	void cobol(const string &cobol);
	void cobol(std::istream &is);
};
//====================================================
//= struct fdg::navigater
//====================================================
struct fdg::navigater : public fdg::fields
{
	int rsize;
	navigater();
	navigater &operator << (const fdg::fields &fields);
	void import(const struct fields &fields);

	void demo(const generic::notify &notify) const;
protected:
	const_iterator import(
		std::set<string> &checker
		, int &offset
		, const_iterator begin
		, const_iterator end
		, int sub = 0
		);
};
//====================================================
#endif//__fdg_h__
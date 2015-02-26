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
	int		lv		;//レベル番号
	string	name	;//名称
	bool	sig		;//符号付き
	string	type	;//データタイプ
	int		left	;//整数部バイト数
	int		right	;//小数部バイト数
	bool	pack	;//バック項目？
	int		occurs	;//OCCURS（繰り返し）数

	string	key		;
	int		sub		;//OCCURSインデックス
	int		real	;//実バイト数
	int		offset	;//レコード内オフセット

	field();
	void translate(const string &ebc, string &sjis) const;
	string translate(const string &ebc) const;
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
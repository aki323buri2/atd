//atd.string.h
#ifndef __ATD_STRING_H__
#define __ATD_STRING_H__
#include "atd.h"
namespace atd {;
struct string; 
struct wstring;
struct strings;
//====================================================
//= atd::string
//====================================================
struct string : public object, public std::string
{
	string();
	string(const char *that);
	string(const std::string &that);
	string(int size, char fill);
	string(iterator begin, iterator end);
	string(const_iterator begin, const_iterator end);

	static string format(const char *format, ...);
	static string enclose(const string &s, const string &left, const string &right);
	static strings explode(const string &s, const string &delimiter);
	static int toint(const string &s);
	static int64 toint64(const string &s);
	static double todouble(const string &s);

	string enclose(const string &left, const string &right) const;
	strings explode(const string &delimiter) const;
	int toint() const;
	int64 toint64() const;
	double todouble() const;

	static string paren(const string &s);
	static string brace(const string &s);
	static string bracket(const string &s);
	static string double_quote(const string &s);
	static string single_quote(const string &s);
	static string double_quotation(const string &s);
	static string single_quotation(const string &s);

	string paren() const;
	string brace() const;
	string bracket() const;
	string double_quote() const;
	string single_quote() const;
	string double_quotation() const;
	string single_quotation() const;

	static wstring mb2wc(const string &mb, UINT codepage);
	static string wc2mb(const wstring &wc, UINT codepage);
	wstring mb2wc(UINT codepage) const;

	static string utf8(const string &sjis);
	static string sjis(const string &utf8);
	string utf8() const;
	string sjis() const;

	string trim(const char *list = " \t\v\r\n") const;
	string pop(int size, int margin = 0);
	string until(const string &until);

	static string json_escape(const string &s);
	string json_escape() const;
};
//====================================================
//= atd::wstring
//====================================================
struct wstring : public object, public std::wstring
{
	wstring();
	wstring(const wchar_t *that);
	wstring(const std::wstring &that);
	wstring(int size, wchar_t fill);
	wstring(iterator begin, iterator end);
	wstring(const_iterator begin, const_iterator end);

	string wc2mb(UINT codepage) const;
};
//====================================================
//= atd::strings
//====================================================
struct strings : public object, public std::vector<string>
{
	strings()																																													{ }
	strings(const string &s0)																																									{ entry(s0); }
	strings(const string &s0, const string &s1)																																					{ entry(s0, s1); }
	strings(const string &s0, const string &s1, const string &s2)																																{ entry(s0, s1, s2); }
	strings(const string &s0, const string &s1, const string &s2, const string &s3)																												{ entry(s0, s1, s2, s3); }
	strings(const string &s0, const string &s1, const string &s2, const string &s3, const string &s4)																							{ entry(s0, s1, s2, s3, s4); }
	strings(const string &s0, const string &s1, const string &s2, const string &s3, const string &s4, const string &s5)																			{ entry(s0, s1, s2, s3, s4, s5); }
	strings(const string &s0, const string &s1, const string &s2, const string &s3, const string &s4, const string &s5, const string &s6)														{ entry(s0, s1, s2, s3, s4, s5, s6); }
	strings(const string &s0, const string &s1, const string &s2, const string &s3, const string &s4, const string &s5, const string &s6, const string &s7)										{ entry(s0, s1, s2, s3, s4, s5, s6, s7); }
	strings(const string &s0, const string &s1, const string &s2, const string &s3, const string &s4, const string &s5, const string &s6, const string &s7, const string &s8)					{ entry(s0, s1, s2, s3, s4, s5, s6, s7, s8); }
	strings(const string &s0, const string &s1, const string &s2, const string &s3, const string &s4, const string &s5, const string &s6, const string &s7, const string &s8, const string &s9)	{ entry(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9); }

	strings &entry(const string &s0)																																									{ return entryf( 1, &s0[0]); }
	strings &entry(const string &s0, const string &s1)																																					{ return entryf( 2, &s0[0], &s1[0]); }
	strings &entry(const string &s0, const string &s1, const string &s2)																																{ return entryf( 3, &s0[0], &s1[0], &s2[0]); }
	strings &entry(const string &s0, const string &s1, const string &s2, const string &s3)																												{ return entryf( 4, &s0[0], &s1[0], &s2[0], &s3[0]); }
	strings &entry(const string &s0, const string &s1, const string &s2, const string &s3, const string &s4)																							{ return entryf( 5, &s0[0], &s1[0], &s2[0], &s3[0], &s4[0]); }
	strings &entry(const string &s0, const string &s1, const string &s2, const string &s3, const string &s4, const string &s5)																			{ return entryf( 6, &s0[0], &s1[0], &s2[0], &s3[0], &s4[0], &s5[0]); }
	strings &entry(const string &s0, const string &s1, const string &s2, const string &s3, const string &s4, const string &s5, const string &s6)														{ return entryf( 7, &s0[0], &s1[0], &s2[0], &s3[0], &s4[0], &s5[0], &s6[0]); }
	strings &entry(const string &s0, const string &s1, const string &s2, const string &s3, const string &s4, const string &s5, const string &s6, const string &s7)										{ return entryf( 8, &s0[0], &s1[0], &s2[0], &s3[0], &s4[0], &s5[0], &s6[0], &s7[0]); }
	strings &entry(const string &s0, const string &s1, const string &s2, const string &s3, const string &s4, const string &s5, const string &s6, const string &s7, const string &s8)					{ return entryf( 9, &s0[0], &s1[0], &s2[0], &s3[0], &s4[0], &s5[0], &s6[0], &s7[0], &s8[0]); }
	strings &entry(const string &s0, const string &s1, const string &s2, const string &s3, const string &s4, const string &s5, const string &s6, const string &s7, const string &s8, const string &s9)	{ return entryf(10, &s0[0], &s1[0], &s2[0], &s3[0], &s4[0], &s5[0], &s6[0], &s7[0], &s8[0], &s9[0]); }

	strings &entryf(int count, ...);

	static string implode(const strings &ss, const string &glue);
	string implode(const string &glue) const;
};
}//namespace atd
#endif//__ATD_STRING_H__
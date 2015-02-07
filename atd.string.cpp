//atd.string.cpp
#include "atd.h"
using namespace atd;
//====================================================
//= atd::string
//====================================================
string::string()										: std::string() { }
string::string(const char *that)						: std::string(that) { }
string::string(const std::string &that)					: std::string(that) { }
string::string(int size, char fill)						: std::string(size, fill) { }
string::string(iterator begin, iterator end)			: std::string(begin, end) { }
string::string(const_iterator begin, const_iterator end): std::string(begin, end) { }
string string::format(const char *format, ...)
{
	va_list va;
	va_start(va, format);
	string s(::_vscprintf(format, va) + 1, 0);
	::vsprintf(&s[0], format, va);
	va_end(va); 

	return s.c_str();
}
string string::enclose(const string &s, const string &left, const string &right)
{
	return left + s + (right.length() ? right : left);
}
strings string::explode(const string &s, const string &delimiter)
{
	strings ss;
	if (!delimiter.length())
	{
		return ss.entry(s);
	}
	size_t offset = 0, pos = npos, dlen = delimiter.length();
	const_iterator b = s.begin(), e = s.end();
	while (true)
	{
		pos = s.find(delimiter, offset);
		if (pos == npos)
		{
			ss.push_back(string(b + offset, e));
			break;
		}
		string s = string(b + offset, b + pos);
		ss.push_back(s);
		offset = pos;
		offset += dlen;
	}
	return ss;
}
int		string::toint	(const string &s) { return :: strtol	(&s[0], 0, 10); }
int64	string::toint64	(const string &s) { return ::_strtoi64	(&s[0], 0, 10); }
double	string::todouble(const string &s) { return :: strtod	(&s[0], 0); }

string string::enclose(const string &left, const string &right) const
{ 
	return enclose(*this, left, right);
}
strings string::explode(const string &delimiter) const
{
	return explode(*this, delimiter);
}
int		string::toint	() const { return toint		(*this); }
int64	string::toint64	() const { return toint64	(*this); }
double	string::todouble() const { return todouble	(*this); }

string string::paren(const string &s) { return enclose(s, LPAREN, RPAREN); }
string string::brace(const string &s) { return enclose(s, LBRACE, RBRACE); }
string string::bracket(const string &s) { return enclose(s, LBRACKET, RBRACKET); }
string string::double_quote(const string &s) { return double_quotation(s); }
string string::single_quote(const string &s) { return single_quotation(s); }
string string::double_quotation(const string &s) { return enclose(s, DOUBLE_QUOTATION, ""); }
string string::single_quotation(const string &s) { return enclose(s, SINGLE_QUOTATION, ""); }

string string::paren() const { return paren(*this); }
string string::brace() const { return brace(*this); }
string string::bracket() const { return bracket(*this); }
string string::double_quote() const { return double_quote(*this); }
string string::single_quote() const { return single_quote(*this); }
string string::double_quotation() const { return double_quotation(*this); }
string string::single_quotation() const { return single_quotation(*this); }

wstring string::mb2wc(const string &mb, UINT codepage)
{
	int (__stdcall *func)(UINT, DWORD, const char *, int, wchar_t *, int) = ::MultiByteToWideChar;
	wstring wc((*func)(codepage, 0, &mb[0], -1, NULL, 0) + 1, 0);
	(*func)(codepage, 0, &mb[0], -1, &wc[0], wc.size());
	return wc.c_str();
}
string string::wc2mb(const wstring &wc, UINT codepage)
{
	int (__stdcall *func)(UINT, DWORD, const wchar_t *, int, char *, int, const char *, BOOL *) = ::WideCharToMultiByte;
	string mb((*func)(codepage, 0, &wc[0], -1, NULL, 0, NULL, NULL) + 1, 0);
	(*func)(codepage, 0, &wc[0], -1, &mb[0], mb.size(), NULL, NULL);
	return mb.c_str();
}
wstring string::mb2wc(UINT codepage) const { return string::mb2wc(*this, codepage); }
string wstring::wc2mb(UINT codepage) const { return string::wc2mb(*this, codepage); }

string string::utf8(const string &sjis) { return wc2mb(mb2wc(sjis, CP_ACP), CP_UTF8); }
string string::sjis(const string &utf8) { return wc2mb(mb2wc(utf8, CP_UTF8), CP_ACP); }
string string::utf8() const { return utf8(*this); }
string string::sjis() const { return sjis(*this); }

static inline int min(int a, int b)
{
	return a < b ? a : b;
}
string string::trim(const char *list) const
{
	const string &s = *this;
	size_t left = s.find_first_not_of(list);
	if (left == npos) return "";
	size_t right = s.find_last_not_of(list);
	return s.substr(left, (right - left + 1));
}
string string::pop(int size, int margin)
{
	string &s = *this;
	int len = s.length();
	//マイナスに対応
	if (size < 0)
	{
		size = len - ::abs(size);
		string r = s.substr(min(size + margin, len));
		s = s.substr(0, size);
		return r;
	}
	else
	{
		string r = s.substr(0, size);
		s = s.substr(min(size + margin, len));
		return r;
	}
}
string string::until(const string &until)
{
	string &s = *this;
	size_t size = s.find(until);
	size_t margin = until.length();
	if (size == npos)
	{
		size = s.length();
		margin = 0;
	}
	return s.pop(size, margin);
}
//====================================================
//= string::json_escape()
//====================================================
string string::json_escape(const string &s)
{
	return s.json_escape();
}
string string::json_escape() const
{
	string s;
	for (const_iterator i = begin(), e = end(); i != e; ++i)
	{
		uchar c = *i;
		switch (c)
		{
			case '\\' : 
			case '"' : 
			case '/' : 
			case '\b' : 
			case '\f' : 
			case '\n' : 
			case '\r' : 
			case '\t' : s += '\\'; break;
		}
		s += c;
	}
	return s;
}

//====================================================
//= atd::wstring
//====================================================
wstring::wstring()											: std::wstring() { }
wstring::wstring(const wchar_t *that)						: std::wstring(that) { }
wstring::wstring(const std::wstring &that)					: std::wstring(that) { }
wstring::wstring(int size, wchar_t fill)					: std::wstring(size, fill) { }
wstring::wstring(iterator begin, iterator end)				: std::wstring(begin, end) { }
wstring::wstring(const_iterator begin, const_iterator end)	: std::wstring(begin, end) { }
//====================================================
//= atd::strings
//====================================================
strings &strings::entryf(int count, ...)
{
	va_list va;
	va_start(va, count);
	for (int i = 0; i < count; i++)
	{
		push_back(va_arg(va, const char *));
	}
	va_end(va);

	return *this;
}
string strings::implode(const strings &ss, const string &glue)
{
	string s;
	for (const_iterator i = ss.begin(), e = ss.end(); i != e; ++i)
	{
		if (s.length()) s += glue;
		s += *i;
	}
	return s;
}
string strings::implode(const string &glue) const
{
	return implode(*this, glue);
}
/*
*/
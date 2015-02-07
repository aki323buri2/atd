//atd.generic.status.cpp
#include "atd.h"
using namespace atd;
using namespace atd::generic;
//====================================================
//= atd::generic::status
//====================================================
status::status(DWORD code, const string &what, const strings &description)
: code(code)
, what(what)
, description(description)
{ }
status::status(const primitive *primitive)
: code(primitive->code)
, what(primitive->what)
{
	for (const char **p = (const char **)primitive->description; *p; p++)
	{
		description.push_back(*p);
	}
}
status status::empty()
{
	return status(0, "", strings());
}
status status::lasterror()
{
	return lasterror(::GetLastError());
}
status status::lasterror(DWORD code)
{
	string s(0x400, 0);//★
	::FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM
		, NULL
		, code
		, 0
		, &s[0]
		, s.size()
		, NULL
	);
	s = string::utf8(s.c_str());//★
	return status(code, s, strings());
}
//★
void status::display(const generic::notify &notify) const
{
	generic::notifyf notifyf = notify;
	notify("------------------------------------------------");
	notifyf("code : %d(0x%x)", code, code);
	notifyf("what : %s", what.c_str());
	notifyf("description: ");
	const strings &dd = description;
	for (strings::const_iterator i = dd.begin(), e = dd.end(); i != e; ++i)
	{
		notifyf("\t%s", i->c_str());
	}
	notify("------------------------------------------------");
}
//====================================================
//= atd::generic::status::book
//====================================================
status::book::book()
{ }
status::book::book(const status::primitive *primitive)
{
	append(primitive);
}
status::book &status::book::append(const status::primitive *primitive)
{
	for (const status::primitive *pp = primitive; pp->what; pp++)
	{
		push(status(pp));
	}
	return *this;
}
status &status::book::operator [](DWORD code)
{
	keys::iterator i = keys.find(code), e = keys.end();
	if (i == e)
	{
		push_back(status(0, "", strings()));
		keys.insert(keys::value_type(code, size() - 1));
		i = keys.find(code);
	}
	return *(begin() + i->second);
}
status &status::book::push(const struct status &status)
{
	struct status &select = (*this)[status.code];
	select = status;
	return select;
}
status status::book::pop(DWORD code, const status &backup)
{
	keys::iterator i = keys.find(code), e = keys.end();
	if (i == e) return backup;

	size_t pos = i->second;
	iterator that = begin() + pos;
	status pop = *that;
	keys.erase(i);
	erase(that);
	for (size_t p = pos, size = this->size(); p < size; p++)
	{
		keys[(begin() + pos)->code] = p;
	}
	return pop;
}
status status::book::status_of(DWORD code, const status &backup) const
{
	keys::const_iterator i = keys.find(code), e = keys.end();
	return i == e ? backup : *(begin() + i->second);
}
void status::book::clear()
{
	std::vector<status>::clear();
	keys.clear();
}
//★
void status::book::display(const generic::notify &notify) const
{
	for (const_iterator i = begin(), e = end(); i != e; ++i)
	{
		i->display(notify);
	}
}
/*
*/

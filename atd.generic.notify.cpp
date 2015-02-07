//atd.generic.notify.cpp
#include "atd.h"
using namespace atd;
using namespace atd::generic;
//====================================================
//= atd::generic::notify
//====================================================
notify::notify(void (*func)(const string &))
: generic::function<void, const string &>(func)
{ }
notify::notify()
: generic::function<void, const string &>()
{ }
//====================================================
//= atd::generic::notifyf
//====================================================
notifyf::notifyf(void (*func)(const string &))
: notify(func)
{ }
notifyf::notifyf()
: notify()
{ }
void notifyf::operator ()(const char *format, ...) const
{
	va_list va;
	va_start(va, format);
	string s(::_vscprintf(format, va) + 1, 0);
	::vsprintf(&s[0], format, va);
	va_end(va);

	return notify::invoke(s.c_str());
}

notifyf::notifyf(const notify &that)
: notify(that)
{ }
/*
*/

//atd.generic.notify.h
#ifndef __ATD_GENERIC_NOTIFY_H__
#define __ATD_GENERIC_NOTIFY_H__
#include "atd.h"
namespace atd {;
namespace generic {;
//====================================================
//= atd::generic::notify
//====================================================
struct notify : public function<void, const string &>
{
	template <typename T>
	notify(void (T::*method)(const string &), T *offset);
	notify(void (*func)(const string &));
	notify();
};
template <typename T>
notify::notify(void (T::*method)(const string &), T *offset)
: generic::function<void, const string &>(method, offset)
{ }
//====================================================
//= atd::generic::notifyf
//====================================================
struct notifyf : public notify
{
	template <typename T>
	notifyf(void (T::*method)(const string &), T *offset);
	notifyf(void (*func)(const string &));
	notifyf();
	void operator ()(const char *format, ...) const;

	notifyf(const notify &that);
};
template <typename T>
notifyf::notifyf(void (T::*method)(const string &), T *offset)
: notify(method, offset)
{ }
}//namespace generic
}//namespace atd
#endif//__ATD_GENERIC_NOTIFY_H__
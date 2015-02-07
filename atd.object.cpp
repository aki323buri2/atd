//atd.object.cpp
#include "atd.h"
using namespace atd;
//====================================================
//= atd::object
//====================================================
//====================================================
//= atd::objects
//====================================================
objects &objects::entryf(int count, ...)
{
	va_list va;
	va_start(va, count);
	for (int i = 0; i < count; i++)
	{
		push_back(va_arg(va, object *));
	}
	va_end(va);

	return *this;
}
void objects::destroy()
{
	for (iterator i = begin(); i != end(); )
	{
		delete *i;
		i = erase(i);
	}
}
//====================================================
//= atd::managed::objects
//====================================================
managed::objects::~objects()
{
	destroy();
}

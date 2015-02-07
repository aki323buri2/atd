//atd.generic.functions.impl.h
#ifndef __ATD_GENERIC_FUNCTIONS_IMPL_H__
#define __ATD_GENERIC_FUNCTIONS_IMPL_H__
#include "atd.h"
namespace atd {;
namespace generic {;
namespace base {;
//====================================================
//= atd::generic::base::functions<R, A>
//====================================================
template <typename R, typename A>
template <typename T>
void functions<R, A>::add(R (T::*method)(A a), T *offset)
{
	return push(function(method, offset));
}
template <typename R, typename A>
template <typename T>
void functions<R, A>::rem(R (T::*method)(A a), T *offset)
{
	return remove(function(method, offset));
}
template <typename R, typename A>
template <typename T>
void functions<R, A>::rep(R (T::*method)(A a), T *offset)
{
	return replace(function(method, offset));
}
template <typename R, typename A>
void functions<R, A>::add(R (*func)(A a))
{
	return push(function(func));
}
template <typename R, typename A>
void functions<R, A>::rem(R (*func)(A a))
{
	return remove(function(func));
}
template <typename R, typename A>
void functions<R, A>::rep(R (*func)(A a))
{
	return replace(function(func));
}
template <typename R, typename A>
void functions<R, A>::push(const function &that)
{
	this->push_back(that);
}
template <typename R, typename A>
void functions<R, A>::remove(const function &that)
{
	iterator b = this->begin(), e = this->end();
	iterator find = std::find(b, e, that);
	if (find != e)
	{
		this->erase(find);
	}
}
template <typename R, typename A>
void functions<R, A>::replace(const function &that)
{
	iterator b = this->begin(), e = this->end();
	iterator find = std::find(b, e, that);
	if (find != e)
	{
		find = that;
	}
	else
	{
		push(that);
	}
}
}//namespace base
//====================================================
//= atd::generic::functions<R, A>
//====================================================
template <typename R, typename A>
R functions<R, A>::operator ()(A a) const
{
	return invoke(a);
}
template <typename R, typename A>
R functions<R, A>::invoke(A a) const
{
	R r = R();
	for (typename functions<R, A>::const_iterator i = this->begin(), e = this->end(); i != e; ++i)
	{
		if (this->handled(a)) break;
		r = i->invoke(a);
	}
	return r;
}
//====================================================
//= atd::generic::functions<void, A>
//====================================================
template <typename A>
void functions<void, A>::operator ()(A a) const
{
	return invoke(a);
}
template <typename A>
void functions<void, A>::invoke(A a) const
{
	for (typename functions<void, A>::const_iterator i = this->begin(), e = this->end(); i != e; ++i)
	{
		if (this->handled(a)) break;
		i->invoke(a);
	}
}
}//namespace generic
}//namespace atd
#endif//__ATD_GENERIC_FUNCTIONS_IMPL_H__
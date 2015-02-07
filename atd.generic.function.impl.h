//atd.generic.function.impl.h
#ifndef __ATD_GENERIC_FUNCTION_IMPL_H__
#define __ATD_GENERIC_FUNCTION_IMPL_H__
#include "atd.h"
namespace atd {;
namespace generic {;
//====================================================
//= atd::generic::function<R, A>
//====================================================
template <typename R, typename A>
template <typename T>
function<R, A>::function(R (T::*method)(A), T *offset)
: func(0), method((method_type)method), offset(offset)
{ }
template <typename R, typename A>
function<R, A>::function(R (*func)(A))
: func(func), method(0), offset(0)
{ }
template <typename R, typename A>
function<R, A>::function()
: func(0), method(0), offset(0)
{ }
template <typename R, typename A>
R function<R, A>::operator ()(A a) const
{
	return invoke(a);
}
template <typename R, typename A>
R function<R, A>::invoke(A a) const
{
	typename function<R, A>::func_type func = this->func;
	typename function<R, A>::method_type method = this->method;
	typename function<R, A>::offset_type offset = this->offset;
	return func ? (*func)(a) : (offset && method ? (offset->*method)(a) : R());
}
template <typename R, typename A>
bool function<R, A>::operator == (const function &that) const
{
	return func == that.func && method == that.method && offset == that.offset;
}
template <typename R, typename A>
bool function<R, A>::operator != (const function &that) const
{
	return !operator = (that);
}
template <typename R, typename A>
bool function<R, A>::operator <  (const function &that) const
{
	return func == that.func ? (method == that.method ? offset < that.offset : (method < that.method)) : func < that.func;
}
//====================================================
//= atd::generic::function<R, void>
//====================================================
template <typename R>
template <typename T>
function<R, void>::function(R (T::*method)(), T *offset)
: func(0), method((method_type)method), offset(offset)
{ }
template <typename R>
function<R, void>::function(R (*func)())
: func(func), method(0), offset(0)
{ }
template <typename R>
function<R, void>::function()
: func(0), method(0), offset(0)
{ }
template <typename R>
R function<R, void>::operator ()() const
{
	return invoke();
}
template <typename R>
R function<R, void>::invoke() const
{
	typename function<R, void>::func_type func = this->func;
	typename function<R, void>::method_type method = this->method;
	typename function<R, void>::offset_type offset = this->offset;
	func ? (*func)() : (offset && method ? (offset->*method)() : (void)0);
}
template <typename R>
bool function<R, void>::operator == (const function &that) const
{
	return func == that.func && method == that.method && offset == that.offset;
}
template <typename R>
bool function<R, void>::operator != (const function &that) const
{
	return !operator = (that);
}
template <typename R>
bool function<R, void>::operator <  (const function &that) const
{
	return func == that.func ? (method == that.method ? offset < that.offset : (method < that.method)) : func < that.func;
}
}//namespace generic
}//namespace atd
#endif//__ATD_GENERIC_FUNCTION_IMPL_H__
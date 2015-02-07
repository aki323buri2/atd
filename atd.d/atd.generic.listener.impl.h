//atd.generic.listener.impl.h
#ifndef __ATD_GENERIC_LISTENER_IMPL_H__
#define __ATD_GENERIC_LISTENER_IMPL_H__
#include "atd.h"
namespace atd {;
namespace generic {;
//====================================================
//= atd::generic::listener
//====================================================
template <typename A>
template <typename T>
void listener<A>::add(key_type key, void (T::*method)(A &a), T *offset)
{
	return push(key, function(method, offset));
}
template <typename A>
template <typename T>
void listener<A>::rem(key_type key, void (T::*method)(A &a), T *offset)
{
	return remove(key, function(method, offset));
}
template <typename A>
template <typename T>
void listener<A>::rep(key_type key, void (T::*method)(A &a), T *offset)
{
	return replace(key, function(method, offset));
}
template <typename A>
void listener<A>::add(key_type key, void (*func)(A &a))
{
	return push(key, function(func));
}
template <typename A>
void listener<A>::rem(key_type key, void (*func)(A &a))
{
	return remove(key, function(func));
}
template <typename A>
void listener<A>::rep(key_type key, void (*func)(A &a))
{
	return replace(key, function(func));
}
template <typename A>
void listener<A>::push(key_type key, const function &that)
{
	(*this)[key].push(that);
}
template <typename A>
void listener<A>::remove(key_type key, const function &that)
{
	if (!key)
	{
		backup.remove(that);
		return;
	}
	functions *funcs = value_of(key);
	if (funcs) funcs->remove(that);
	return;
}
template <typename A>
void listener<A>::replace(key_type key, const function &that)
{
	function &func = (*this)[key];
	return func.replace(that);
}
template <typename A>
bool listener<A>::has_key(key_type key) const
{
	return this->find(key) != this->end();
}
template <typename A>
typename listener<A>::functions *listener<A>::value_of(key_type key) const
{
	typename listener<A>::const_iterator i = this->find(key), e = this->end();
	return i == e ? 0 : (functions *)&(i->second);
}
template <typename A>
typename listener<A>::functions &listener<A>::operator [](key_type key)
{
	if (!key) return backup;
	typename listener<A>::iterator i = this->find(key), e = this->end();
	if (i == e)
	{
		this->insert(value_type(key, functions()));
		i = this->find(key);
	}
	return i->second;
}

template <typename A>
void listener<A>::fire(key_type key, A &a) const
{
	functions *funcs = value_of(key);
	if (funcs)
	{
		(*funcs)(a);
	}
	if (!functions::handled(a))
	{
		backup(a);
	}
}
}//namespace generic
}//namespace atd
#endif//__ATD_GENERIC_LISTENER_IMPL_H__
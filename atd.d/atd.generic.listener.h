//atd.generic.listener.h
#ifndef __ATD_GENERIC_LISTENER_H__
#define __ATD_GENERIC_LISTENER_H__
#include "atd.h"
namespace atd {;
namespace generic {;
//====================================================
//= atd::generic::listener<A>
//====================================================
template <typename A>
struct listener : public object, public std::map<int, functions<void, A &> >
{
	typedef typename std::map<int, generic::functions<void, A &> > root_type;
	typedef typename root_type::key_type key_type;
	typedef typename root_type::value_type value_type;
	typedef typename generic::functions<void, A &> functions;
	typedef typename generic::function<void, A &> function;
	template <typename T>
	void add(key_type key, void (T::*method)(A &a), T *offset);
	template <typename T>
	void rem(key_type key, void (T::*method)(A &a), T *offset);
	template <typename T>
	void rep(key_type key, void (T::*method)(A &a), T *offset);
	void add(key_type key, void (*func)(A &a));
	void rem(key_type key, void (*func)(A &a));
	void rep(key_type key, void (*func)(A &a));
	void push(key_type key, const function &that);
	void remove(key_type key, const function &that);
	void replace(key_type key, const function &that);

	functions backup;
	bool has_key(key_type key) const;
	functions *value_of(key_type key) const;
	functions &operator [](key_type key);

	void fire(key_type key, A &a) const;
};
}//namespace generic
}//namespace atd
//====================================================
//= atd::generic::listener implementation include
//====================================================
#include "atd.generic.listener.impl.h"
#endif//__ATD_GENERIC_LISTENER_H__
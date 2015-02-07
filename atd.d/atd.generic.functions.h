//atd.generic.functions.h
#ifndef __ATD_GENERIC_FUNCTIONS_H__
#define __ATD_GENERIC_FUNCTIONS_H__
#include "atd.h"
namespace atd {;
namespace generic {;
namespace base {;
//====================================================
//= atd::generic::base::functions<R, A>
//====================================================
template <typename R, typename A>
struct functions : public object, public std::vector<generic::function<R, A> >
{
	typedef typename functions<R, A>::value_type function;
	typedef typename functions<R, A>::iterator iterator;
	template <typename T>
	void add(R (T::*method)(A a), T *offset);
	template <typename T>
	void rem(R (T::*method)(A a), T *offset);
	template <typename T>
	void rep(R (T::*method)(A a), T *offset);
	void add(R (*func)(A a));
	void rem(R (*func)(A a));
	void rep(R (*func)(A a));
	void push(const function &that);
	void remove(const function &that);
	void replace(const function &that);

	struct has_handled
	{
		template <typename T, bool T::*>
		struct tester
		{
			typedef void type;
		};
		template <typename T, typename U = void>
		struct test
		{
			static bool handled(T &t) { return false; }
		};
		template <typename T>
		struct test<T &, typename tester<T, &T::handled>::type>
		{
			static bool handled(T &t) { return t.handled; }
		};
		static bool handled(A &a) { return test<A>::handled(a); }
	};
	static bool handled(A &a) { return has_handled::handled(a); }
};
}//namespace base
//====================================================
//= atd::generic::functions<R, A>
//====================================================
template <typename R, typename A>
struct functions : public base::functions<R, A>
{
	R operator ()(A a) const;
	R invoke(A a) const;
};
//====================================================
//= atd::generic::functions<void, A>
//====================================================
template <typename A>
struct functions<void, A> : public base::functions<void, A>
{
	void operator ()(A a) const;
	void invoke(A a) const;
};
}//namespace generic
}//namespace atd
//====================================================
//= atd::generic::functions implementation include
//====================================================
#include "atd.generic.functions.impl.h"
#endif//__ATD_GENERIC_FUNCTIONS_H__
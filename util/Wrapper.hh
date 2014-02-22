#ifndef REACTOR_UTIL_WRAPPER_HEADER
#define REACTOR_UTIL_WRAPPER_HEADER

#include <util/SharedPtr.hh>

namespace util {

template <class, class, class> class Wrapper;

template <class T, class P, class S>
class WrapperProxy {
	T& t_;
	S& s_;
	mutable bool o_;

	WrapperProxy& operator=(const WrapperProxy&);
	WrapperProxy(T& t, P& p, S& s) : t_(t), s_(s), o_(true) { p(); }
	friend class Wrapper<T, P, S>;

public:
	WrapperProxy(const WrapperProxy& o) : t_(o.t_), s_(o.s_), o_(o.o_) { o.o_ = false; }
	~WrapperProxy() { if (o_) s_(); }

	T* operator->() { return &t_; }
};

template <class T, class P, class S>
class Wrapper {
	SharedPtr<T> t_;
	P p_;
	S s_;

public:
	Wrapper(T& t, const P& p, const S& s) : t_(t), p_(p), s_(s) {}
	Wrapper(T* t, const P& p, const S& s) : t_(t), p_(p), s_(s) {}

	WrapperProxy<T, P, S> operator->() { return WrapperProxy<T, P, S>(*t_, p_, s_); }
	T& direct() { return *t_; }
};

} // namespace util

#endif // REACTOR_UTIL_WRAPPER_HEADER

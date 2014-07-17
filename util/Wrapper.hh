#ifndef REACTOR_UTIL_WRAPPER_HEADER
#define REACTOR_UTIL_WRAPPER_HEADER

#include <util/SharedPtr.hh>

namespace util {

template <class T, class P, class S>
class Wrapper {
private:
	class Proxy {
	public:
		Proxy(T& t, P p, S s) : t_(t), s_(s), o_(true) { p(); }
		Proxy(const Proxy& o) : t_(o.t_), s_(o.s_), o_(o.o_) { o.o_ = false; }
		~Proxy() { if (o_) s_(); }

		T* operator->() { return &t_; }

	private:
		Proxy& operator=(const Proxy&);

		T t_;
		S s_;
		mutable bool o_;
	};

public:
	Wrapper(T& t, P p, S s) : t_(t), p_(p), s_(s) {}
	Wrapper(T* t, P p, S s) : t_(t), p_(p), s_(s) {}

	Proxy operator->() { return Proxy(*t_, p_, s_); }
	T& direct() { return *t_; }

private:
	SharedPtr<T> t_;
	P p_;
	S s_;
};

} // namespace util

#endif // REACTOR_UTIL_WRAPPER_HEADER

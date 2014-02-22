#ifndef REACTOR_UTIL_SHAREDPTR_HEADER
#define REACTOR_UTIL_SHAREDPTR_HEADER

namespace util {

template <typename T>
class SharedPtr {
	T* t_;
	mutable int *c_;

	void inc() const { if (c_) ++*c_; }
	void dec() { if (c_ && !--*c_) { delete c_; delete t_; } }

public:
	explicit SharedPtr(T& t) : t_(&t), c_(0) {}
	explicit SharedPtr(T* t = 0) : t_(t), c_(t_ ? new int(1) : 0) {}
	SharedPtr(const SharedPtr& o) : t_(o.t_), c_(o.c_) { inc(); }
	SharedPtr& operator=(const SharedPtr& o) { o.inc(); dec(); t_ = o.t_; c_ = o.c_; return *this; }
	~SharedPtr() { dec(); }

	T* get() { return t_; }
	const T* get() const { return t_; }

	void reset(T *t = 0) { operator=(SharedPtr(t)); }

	T& operator*() { return *t_; }
	const T& operator*() const { return *t_; }

	T* operator->() { return t_; }
	const T* operator->() const { return t_; }

	int use_count() const { return c_ ? *c_ : 0; }
	bool unique() const { return c_ ? (*c_ == 1) : false; }
};

} // namespace util

#endif // REACTOR_UTIL_SHAREDPTR_HEADER

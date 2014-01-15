#ifndef REACTOR_THREAD_GUARD_HEADER
#define REACTOR_THREAD_GUARD_HEADER

namespace thread {

template <class L>
class Guard {
	L &lock_;

public:
	Guard(L &lock)
	: lock_(lock)
	{
		lock_.acquire();
	}

	~Guard()
	{
		lock_.release();
	}
};

} // namespace thread

#endif // REACTOR_THREAD_GUARD_HEADER

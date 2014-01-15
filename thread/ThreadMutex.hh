#ifndef REACTOR_THREAD_THREADMUTEX_HEADER
#define REACTOR_THREAD_THREADMUTEX_HEADER

#include <util/Noncopyable.hh>

namespace thread {

class ThreadMutexImpl;

class ThreadMutex : public util::Noncopyable {
	ThreadMutexImpl *impl_;

	friend class ThreadCondition;

public:
	ThreadMutex();
	~ThreadMutex();

	void acquire();
	void release();
};

} // namespace thread

#endif // REACTOR_THREAD_THREADMUTEX_HEADER

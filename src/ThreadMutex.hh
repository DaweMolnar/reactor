#ifndef REACTOR_THREADMUTEX_HEADER
#define REACTOR_THREADMUTEX_HEADER

#include <util/Noncopyable.hh>

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

#endif // REACTOR_THREADMUTEX_HEADER

#ifndef REACTOR_THREAD_THREADCONDITION_HEADER
#define REACTOR_THREAD_THREADCONDITION_HEADER

#include <thread/ThreadMutex.hh>

#include <util/Noncopyable.hh>

class ThreadConditionImpl;

class ThreadCondition : public util::Noncopyable {
	ThreadConditionImpl *impl_;

public:
	ThreadCondition(ThreadMutex &mutex);
	~ThreadCondition();

	void wait();
	void notify();
	void notifyAll();
};

#endif // REACTOR_THREAD_THREADCONDITION_HEADER

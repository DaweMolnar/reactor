#ifndef REACTOR_THREADCONDITION_HEADER
#define REACTOR_THREADCONDITION_HEADER

#include "ThreadMutex.hh"

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

#endif // REACTOR_THREADCONDITION_HEADER

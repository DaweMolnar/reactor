#ifndef REACTOR_THREAD_THREADCONDITION_HEADER
#define REACTOR_THREAD_THREADCONDITION_HEADER

#include <thread/ThreadMutex.hh>

#include <util/Noncopyable.hh>

namespace thread {

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

} // namespace thread

#endif // REACTOR_THREAD_THREADCONDITION_HEADER

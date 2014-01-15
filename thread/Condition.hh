#ifndef REACTOR_THREAD_CONDITION_HEADER
#define REACTOR_THREAD_CONDITION_HEADER

#include <thread/Mutex.hh>

#include <util/Noncopyable.hh>

namespace thread {

class ConditionImpl;

class Condition : public util::Noncopyable {
	ConditionImpl *impl_;

public:
	Condition(Mutex &mutex);
	~Condition();

	void wait();
	void notify();
	void notifyAll();
};

} // namespace thread

#endif // REACTOR_THREAD_CONDITION_HEADER

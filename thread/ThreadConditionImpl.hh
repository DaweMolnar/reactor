#ifndef REACTOR_THREADCONDITIONIMPL_HEADER
#define REACTOR_THREADCONDITIONIMPL_HEADER

namespace thread {

class ThreadConditionImpl {
public:
	virtual ~ThreadConditionImpl() {}

	virtual void wait() = 0;
	virtual void notify() = 0;
	virtual void notifyAll() = 0;
};

} // namespace thread

#endif // REACTOR_THREADCONDITIONIMPL_HEADER

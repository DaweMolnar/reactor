#ifndef REACTOR_THREAD_CONDITIONIMPL_HEADER
#define REACTOR_THREAD_CONDITIONIMPL_HEADER

namespace thread {

class ConditionImpl {
public:
	virtual ~ConditionImpl() {}

	virtual void wait() = 0;
	virtual void notify() = 0;
	virtual void notifyAll() = 0;
};

} // namespace thread

#endif // REACTOR_THREAD_CONDITIONIMPL_HEADER

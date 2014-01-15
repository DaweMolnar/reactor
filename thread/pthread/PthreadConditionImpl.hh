#ifndef REACTOR_THREAD_PTHREAD_PTHREADCONDITIONIMPL_HEADER
#define REACTOR_THREAD_PTHREAD_PTHREADCONDITIONIMPL_HEADER

#include "../ConditionImpl.hh"
#include "../MutexImpl.hh"

#include <pthread.h>

namespace thread {
namespace pthread {

class PthreadConditionImpl : public ConditionImpl {
	pthread_mutex_t &mutex_;
	pthread_cond_t cond_;

public:
	PthreadConditionImpl(MutexImpl &mutexImpl);
	~PthreadConditionImpl();

	void wait();
	void notify();
	void notifyAll();
};

} // namespace pthread
} // namespace thread

#endif // REACTOR_THREAD_PTHREAD_PTHREADCONDITIONIMPL_HEADER

#ifndef REACTOR_PTHREAD_PTHREADCONDITIONIMPL_HEADER
#define REACTOR_PTHREAD_PTHREADCONDITIONIMPL_HEADER

#include "../ThreadConditionImpl.hh"
#include "../ThreadMutexImpl.hh"

#include <pthread.h>

namespace thread {
namespace pthread {

class PthreadConditionImpl : public ThreadConditionImpl {
	pthread_mutex_t &mutex_;
	pthread_cond_t cond_;

public:
	PthreadConditionImpl(ThreadMutexImpl &mutexImpl);
	~PthreadConditionImpl();

	void wait();
	void notify();
	void notifyAll();
};

} // namespace pthread
} // namespace thread

#endif // REACTOR_PTHREAD_PTHREADCONDITIONIMPL_HEADER

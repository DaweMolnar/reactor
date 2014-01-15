#ifndef REACTOR_THREAD_PTHREAD_THREADMUTEXIMPL_HEADER
#define REACTOR_THREAD_PTHREAD_THREADMUTEXIMPL_HEADER

#include "../MutexImpl.hh"

#include <pthread.h>

namespace thread {
namespace pthread {

class PthreadMutexImpl : public MutexImpl {
	pthread_mutex_t mutex_;

	friend class PthreadConditionImpl;

public:
	PthreadMutexImpl();
	~PthreadMutexImpl();

	virtual void acquire();
	virtual void release();
};

} // namespace pthread
} // namespace thread

#endif // REACTOR_THREAD_PTHREAD_THREADMUTEXIMPL_HEADER

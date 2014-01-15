#ifndef REACTOR_PTHREAD_THREADMUTEXIMPL_HEADER
#define REACTOR_PTHREAD_THREADMUTEXIMPL_HEADER

#include "../ThreadMutexImpl.hh"

#include <pthread.h>

namespace thread {
namespace pthread {

class PthreadMutexImpl : public ThreadMutexImpl {
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

#endif // REACTOR_PTHREAD_THREADMUTEXIMPL_HEADER

#include "Thread.hh"

#include <pthread.h>
#include <stdexcept>

struct ThreadImpl {
	pthread_t thread;
};

union Castor {
	void *pthreadArg;
	Thread *thread;
};

void *
Thread::routine(void *arg)
{
	Castor c;
	c.pthreadArg = arg;
	c.thread->runnable_.run();
	return 0;
}

Thread::Thread(Runnable &runnable)
: impl_(new ThreadImpl())
, runnable_(runnable)
{
	int error;
	Castor c;

	c.thread = this;
	error = pthread_create(&impl_->thread, 0, &Thread::routine, c.pthreadArg);
	if (error) {
		throw std::runtime_error("failed to create thread");
	}
}

Thread::~Thread()
{
	int error = pthread_join(impl_->thread, 0);

	if (error) {
		// XXX: throw in destructor?
		throw std::runtime_error("failed to join thread");
	}
}

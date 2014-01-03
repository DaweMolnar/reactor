#include "Thread.hh"

#include <pthread.h>
#include <stdexcept>

class ThreadImpl {
	union Castor {
		void *pthreadArg;
		ThreadImpl *threadImpl;
	};

	pthread_t thread_;
	Runnable &runnable_;

	static void *routine(void *arg);

public:
	ThreadImpl(Runnable &runnable);
	~ThreadImpl();
};

void *
ThreadImpl::routine(void *arg)
{
	Castor c;
	c.pthreadArg = arg;
	c.threadImpl->runnable_.run();
	return 0;
}

ThreadImpl::ThreadImpl(Runnable &runnable)
: runnable_(runnable)
{
	int error;
	Castor c;

	c.threadImpl = this;
	error = pthread_create(&thread_, 0, &ThreadImpl::routine, c.pthreadArg);
	if (error) {
		throw std::runtime_error("failed to create thread");
	}
}

ThreadImpl::~ThreadImpl()
{
	int error = pthread_join(thread_, 0);

	if (error) {
		// XXX: throw in destructor?
		throw std::runtime_error("failed to join thread");
	}
}

Thread::Thread(Runnable &runnable)
: impl_(new ThreadImpl(runnable))
{}

Thread::~Thread()
{
	delete impl_;
}

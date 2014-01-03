#include "ThreadMutex.hh"

#include <pthread.h>
#include <stdexcept>

class ThreadMutexImpl {
	pthread_mutex_t mutex_;

public:
	ThreadMutexImpl();
	~ThreadMutexImpl();

	void acquire();
	void release();
};

ThreadMutexImpl::ThreadMutexImpl()
{
	int error = pthread_mutex_init(&mutex_, 0);
	if (error) {
		throw std::runtime_error("failed to initialize mutex");
	}
}

ThreadMutexImpl::~ThreadMutexImpl()
{
	int error = pthread_mutex_destroy(&mutex_);
	if (error) {
		throw std::runtime_error("failed to destroy mutex");
	}
}

void
ThreadMutexImpl::acquire()
{
	int error = pthread_mutex_lock(&mutex_);
	if (error) {
		throw std::runtime_error("failed to lock mutex");
	}
}

void
ThreadMutexImpl::release()
{
	int error = pthread_mutex_unlock(&mutex_);
	if (error) {
		throw std::runtime_error("failed to unlock mutex");
	}
}


ThreadMutex::ThreadMutex()
: impl_(new ThreadMutexImpl())
{}

ThreadMutex::~ThreadMutex()
{
	delete impl_;
}

void
ThreadMutex::acquire()
{
	impl_->acquire();
}

void
ThreadMutex::release()
{
	impl_->release();
}

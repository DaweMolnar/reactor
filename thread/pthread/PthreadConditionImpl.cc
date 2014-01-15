#include "PthreadConditionImpl.hh"

#include "PthreadMutexImpl.hh"

#include <util/ErrnoException.hh>

#include <iostream>

using namespace thread::pthread;

PthreadConditionImpl::PthreadConditionImpl(ThreadMutexImpl &mutex)
: mutex_(dynamic_cast<PthreadMutexImpl &>(mutex).mutex_)
{
	if (int error = pthread_cond_init(&cond_, 0)) {
		throw util::ErrnoException("pthread_cond_init", error);
	}
}

PthreadConditionImpl::~PthreadConditionImpl()
{
	if (int error = pthread_cond_destroy(&cond_)) {
		util::ErrnoException e("pthread_cond_destroy", error);
		if (std::uncaught_exception()) {
			std::cerr << e.what() << std::endl;
		} else {
			// XXX: throw in destructor?
			throw e;
		}
	}
}

void
PthreadConditionImpl::wait()
{
	if (int error = pthread_cond_wait(&cond_, &mutex_)) {
		throw util::ErrnoException("pthread_cond_wait", error);
	}
}

void
PthreadConditionImpl::notify()
{
	if (int error = pthread_cond_signal(&cond_)) {
		throw util::ErrnoException("pthread_cond_signal", error);
	}
}

void
PthreadConditionImpl::notifyAll()
{
	if (int error = pthread_cond_broadcast(&cond_)) {
		throw util::ErrnoException("pthread_cond_broadcast", error);
	}
}

#include "PthreadMutexImpl.hh"

#include "../ErrnoException.hh"

#include <iostream>

using namespace pthread;

PthreadMutexImpl::PthreadMutexImpl()
{
	if (int error = pthread_mutex_init(&mutex_, 0)) {
		throw ErrnoException("pthread_mutex_init", error);
	}
}

PthreadMutexImpl::~PthreadMutexImpl()
{
	if (int error = pthread_mutex_destroy(&mutex_)) {
		ErrnoException e("pthread_mutex_destroy", error);
		if (std::uncaught_exception()) {
			std::cerr << e.what() << std::endl;
		} else {
			// XXX: throw in destructor?
			throw e;
		}
	}
}

void
PthreadMutexImpl::acquire()
{
	if (int error = pthread_mutex_lock(&mutex_)) {
		throw ErrnoException("pthread_mutex_lock", error);
	}
}

void
PthreadMutexImpl::release()
{
	if (int error = pthread_mutex_unlock(&mutex_)) {
		throw ErrnoException("pthread_mutex_unlock", error);
	}
}
#include "PthreadImpl.hh"

#include "../ErrnoException.hh"

#include <iostream>

using namespace pthread;

void *
PthreadImpl::routine(void *arg)
{
	Castor c;

	c.pthreadArg = arg;
	c.pthreadImpl->run();
	return 0;
}

PthreadImpl::PthreadImpl(Runnable &runnable)
: runnable_(runnable)
, finished_(false)
{
	Castor c;

	c.pthreadImpl = this;
	if (int error = pthread_create(&thread_, 0, &PthreadImpl::routine, c.pthreadArg)) {
		throw ErrnoException("pthread_create", error);
	}
}

PthreadImpl::~PthreadImpl()
{
	if (int error = pthread_join(thread_, 0)) {
		ErrnoException e("pthread_join", error);
		if (std::uncaught_exception()) {
			std::cerr << e.what() << std::endl;
		} else {
			// XXX: throw in destructor?
			throw e;
		}
	}
}

void
PthreadImpl::run()
{
	runnable_.run();
	finished_ = true;
}

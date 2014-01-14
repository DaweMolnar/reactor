#include "PthreadImpl.hh"

#include <util/ErrnoException.hh>

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

PthreadImpl::PthreadImpl(util::Runnable &runnable)
: runnable_(runnable)
, finished_(false)
{
	Castor c;

	c.pthreadImpl = this;
	if (int error = pthread_create(&thread_, 0, &PthreadImpl::routine, c.pthreadArg)) {
		throw util::ErrnoException("pthread_create", error);
	}
}

PthreadImpl::~PthreadImpl()
{
	if (int error = pthread_join(thread_, 0)) {
		util::ErrnoException e("pthread_join", error);
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

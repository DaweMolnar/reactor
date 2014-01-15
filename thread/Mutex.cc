#include "Mutex.hh"

#include "pthread/PthreadMutexImpl.hh"

using namespace thread;

Mutex::Mutex()
: impl_(new pthread::PthreadMutexImpl())
{}

Mutex::~Mutex()
{
	delete impl_;
}

void
Mutex::acquire()
{
	impl_->acquire();
}

void
Mutex::release()
{
	impl_->release();
}

#include "Mutex.hh"

#include "ConcreteImplFactory.hh"

using namespace thread;

Mutex::Mutex()
: impl_(ConcreteImplFactory::getInstance().createMutexImpl())
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

#include "Thread.hh"

#include "ConcreteImplFactory.hh"

using namespace thread;

Thread::Thread(util::Runnable &runnable)
: impl_(ConcreteImplFactory::getInstance().createThreadImpl(runnable))
{}

Thread::~Thread()
{
	delete impl_;
}

bool
Thread::hasFinished()
const
{
	return impl_->hasFinished();
}

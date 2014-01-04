#include "Thread.hh"

#include "pthread/PthreadImpl.hh"

Thread::Thread(Runnable &runnable)
: impl_(new pthread::PthreadImpl(runnable))
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

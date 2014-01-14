#include "ThreadCondition.hh"

#include "pthread/PthreadConditionImpl.hh"

ThreadCondition::ThreadCondition(ThreadMutex &mutex)
: impl_(new pthread::PthreadConditionImpl(*mutex.impl_))
{}

ThreadCondition::~ThreadCondition()
{
	delete impl_;
}

void
ThreadCondition::wait()
{
	impl_->wait();
}

void
ThreadCondition::notify()
{
	impl_->notify();
}

void
ThreadCondition::notifyAll()
{
	impl_->notifyAll();
}

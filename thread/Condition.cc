#include "Condition.hh"

#include "pthread/PthreadConditionImpl.hh"

using namespace thread;

Condition::Condition(Mutex &mutex)
: impl_(new pthread::PthreadConditionImpl(*mutex.impl_))
{}

Condition::~Condition()
{
	delete impl_;
}

void
Condition::wait()
{
	impl_->wait();
}

void
Condition::notify()
{
	impl_->notify();
}

void
Condition::notifyAll()
{
	impl_->notifyAll();
}

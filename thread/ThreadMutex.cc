#include "ThreadMutex.hh"

#include "pthread/PthreadMutexImpl.hh"

ThreadMutex::ThreadMutex()
: impl_(new pthread::PthreadMutexImpl())
{}

ThreadMutex::~ThreadMutex()
{
	delete impl_;
}

void
ThreadMutex::acquire()
{
	impl_->acquire();
}

void
ThreadMutex::release()
{
	impl_->release();
}

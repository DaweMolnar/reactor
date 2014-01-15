#include "PthreadImplFactory.hh"

#include "PthreadImpl.hh"
#include "PthreadMutexImpl.hh"
#include "PthreadConditionImpl.hh"

using namespace thread;
using namespace thread::pthread;

ThreadImpl *
PthreadImplFactory::createThreadImpl(util::Runnable &runnable)
const
{
	return new PthreadImpl(runnable);
}

MutexImpl *
PthreadImplFactory::createMutexImpl()
const
{
	return new PthreadMutexImpl();
}

ConditionImpl *
PthreadImplFactory::createConditionImpl(MutexImpl &mutex)
const
{
	return new PthreadConditionImpl(mutex);
}

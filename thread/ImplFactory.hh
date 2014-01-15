#ifndef REACTOR_THREAD_IMPLFACTORY_HEADER
#define REACTOR_THREAD_IMPLFACTORY_HEADER

#include <thread/ThreadImpl.hh>
#include <thread/MutexImpl.hh>
#include <thread/ConditionImpl.hh>
#include <util/Runnable.hh>

namespace thread {

struct ImplFactory {
	virtual ~ImplFactory() {}

	virtual ThreadImpl *createThreadImpl(util::Runnable &runnable) const = 0;
	virtual MutexImpl *createMutexImpl() const = 0;
	virtual ConditionImpl *createConditionImpl(MutexImpl &mutex) const = 0;
};

} // namespace thread

#endif // REACTOR_THREAD_IMPLFACTORY_HEADER

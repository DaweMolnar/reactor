#ifndef REACTOR_THREAD_PTHREAD_PTHREADIMPLFACTORY_HEADER
#define REACTOR_THREAD_PTHREAD_PTHREADIMPLFACTORY_HEADER

#include <thread/ImplFactory.hh>

namespace thread {
namespace pthread {

struct PthreadImplFactory : public ImplFactory {
	virtual ThreadImpl *createThreadImpl(util::Runnable &runnable) const;
	virtual MutexImpl *createMutexImpl() const;
	virtual ConditionImpl *createConditionImpl(MutexImpl &mutex) const;
};

} // namespace pthread
} // namespace thread

#endif // REACTOR_THREAD_PTHREAD_PTHREADIMPLFACTORY_HEADER

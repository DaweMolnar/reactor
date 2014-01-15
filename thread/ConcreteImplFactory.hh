#ifndef REACTOR_THREAD_CONCRETEIMPLFACTORY_HEADER
#define REACTOR_THREAD_CONCRETEIMPLFACTORY_HEADER

#include <thread/ImplFactory.hh>

namespace thread {

struct ConcreteImplFactory {
	static ImplFactory &getInstance();
};

} // namespace thread

#endif // REACTOR_THREAD_CONCRETEIMPLFACTORY_HEADER

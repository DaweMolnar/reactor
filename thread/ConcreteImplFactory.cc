#include "ConcreteImplFactory.hh"

#include "pthread/PthreadImplFactory.hh"

using namespace thread;

ImplFactory &
ConcreteImplFactory::getInstance()
{
	static pthread::PthreadImplFactory instance;

	return instance;
}

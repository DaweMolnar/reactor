#include "Reactor.hh"

#include "Dispatcher.hh"

#include <cstdlib>

using namespace reactor;

int
Reactor::loop()
{
	while (!quit_) {
		Dispatcher::instance().stepSingleThread();
	}

	return EXIT_SUCCESS;
}

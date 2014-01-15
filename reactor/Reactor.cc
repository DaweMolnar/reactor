#include "Reactor.hh"

#include <cstdlib>

using namespace reactor;

int
Reactor::loop()
{
	while (!quit_) {
		dispatcher_.stepSingleThread();
	}

	return EXIT_SUCCESS;
}

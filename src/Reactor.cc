#include "Reactor.hh"

#include <cstdlib>

int
Reactor::loop()
{
	while (!quit_) {
		dispatcher_.stepSingleThread();
	}

	return EXIT_SUCCESS;
}

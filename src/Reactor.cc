#include "Reactor.hh"

#include <cstdlib>

int
Reactor::run()
{
	while (!quit_) {
		dispatcher_.stepSingleThread();
	}

	return EXIT_SUCCESS;
}

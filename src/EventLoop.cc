#include "EventLoop.hh"

#include <cstdlib>

int
EventLoop::run()
{
	while (!quit_) {
		dispatcher_.stepSingleThread();
	}

	return EXIT_SUCCESS;
}

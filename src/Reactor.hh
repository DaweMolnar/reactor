#ifndef REACTOR_REACTOR_HEADER
#define REACTOR_REACTOR_HEADER

#include "Dispatcher.hh"

class Reactor {
	Dispatcher &dispatcher_;
	bool quit_;

public:
	Reactor(Dispatcher &dispatcher)
	: dispatcher_(dispatcher)
	, quit_(false)
	{}

	void quit() { quit_ = true; }
	int loop();
};

#endif // REACTOR_REACTOR_HEADER

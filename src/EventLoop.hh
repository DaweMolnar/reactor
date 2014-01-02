#ifndef REACTOR_EVENTLOOP_HEADER
#define REACTOR_EVENTLOOP_HEADER

#include "Dispatcher.hh"

class EventLoop {
	Dispatcher &dispatcher_;
	bool quit_;

public:
	EventLoop(Dispatcher &dispatcher)
	: dispatcher_(dispatcher)
	, quit_(false)
	{}

	void quit() { quit_ = true; }
	int run();
};

#endif // REACTOR_EVENTLOOP_HEADER

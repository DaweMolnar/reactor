#ifndef REACTOR_REACTOR_REACTOR_REACTOR_HEADER
#define REACTOR_REACTOR_REACTOR_REACTOR_HEADER

#include <reactor/Dispatcher.hh>

class Reactor {
protected:
	Dispatcher &dispatcher_;
	volatile bool quit_;

public:
	explicit Reactor(Dispatcher &dispatcher)
	: dispatcher_(dispatcher)
	, quit_(false)
	{}

	void quit() { quit_ = true; }
	int loop();
};

#endif // REACTOR_REACTOR_REACTOR_REACTOR_HEADER

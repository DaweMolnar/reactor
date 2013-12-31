#ifndef REACTOR_TIMEREVENT_HEADER
#define REACTOR_TIMEREVENT_HEADER

#include "Timer.hh"

struct TimerEvent {
	Timer timer;

	TimerEvent(const Timer &timer0)
	: timer(timer0)
	{}
};

#endif // REACTOR_TIMEREVENT_HEADER

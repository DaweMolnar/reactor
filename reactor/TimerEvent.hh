#ifndef REACTOR_REACTOR_TIMEREVENT_HEADER
#define REACTOR_REACTOR_TIMEREVENT_HEADER

#include <reactor/Timer.hh>

struct TimerEvent {
	Timer timer;

	explicit TimerEvent(const Timer &timer0)
	: timer(timer0)
	{}
};

#endif // REACTOR_REACTOR_TIMEREVENT_HEADER

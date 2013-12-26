#ifndef REACTOR_LAZYTIMER_HEADER
#define REACTOR_LAZYTIMER_HEADER

#include "Timer.hh"

class LazyTimer : public Timer {
public:
	LazyTimer(const DiffTime &interval, size_t iterationLimit = 0, const Time &t0 = Time::now())
	: Timer(interval, iterationLimit, t0)
	{}
};

#endif // REACTOR_LAZYTIMER_HEADER

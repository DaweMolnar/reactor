#ifndef REACTOR_LAZYTIMER_HEADER
#define REACTOR_LAZYTIMER_HEADER

#include "Timer.hh"

class LazyTimer : public Timer {
public:
	LazyTimer(const util::DiffTime &interval, size_t iterationLimit = 0, const util::Time &t0 = util::Time::now())
	: Timer(interval, iterationLimit, t0)
	{}
};

#endif // REACTOR_LAZYTIMER_HEADER

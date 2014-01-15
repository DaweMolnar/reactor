#ifndef REACTOR_REACTOR_LAZYTIMER_HEADER
#define REACTOR_REACTOR_LAZYTIMER_HEADER

#include <reactor/Timer.hh>

namespace reactor {

class LazyTimer : public Timer {
public:
	LazyTimer(const util::DiffTime &interval, size_t iterationLimit = 0, const util::Time &t0 = util::Time::now())
	: Timer(interval, iterationLimit, t0)
	{}
};

} // namespace reactor

#endif // REACTOR_REACTOR_LAZYTIMER_HEADER

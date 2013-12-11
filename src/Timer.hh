#ifndef REACTOR_TIMER_HEADER
#define REACTOR_TIMER_HEADER

#include "Time.hh"

class Timer {
	DiffTime interval_;
	Time expiration_;
	size_t counter_;
	size_t iterationLimit_;

protected:
	bool lazy_;

public:
	Timer(const DiffTime &interval, size_t iterationLimit = 0)
	: interval_(interval)
	, expiration_(Time::now() + interval)
	, counter_(0)
	, iterationLimit_(iterationLimit)
	, lazy_(false)
	{}

	bool operator<(const Timer &rhs) const { return expiration_ < rhs.expiration_; }

	bool next();

	const Time &expiration() const { return expiration_; }
	bool lazy() const { return lazy_; }
};

#endif // REACTOR_TIMER_HEADER

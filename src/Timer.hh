#ifndef REACTOR_TIMER_HEADER
#define REACTOR_TIMER_HEADER

#include <util/Time.hh>

#include <stddef.h>

class Timer {
	util::DiffTime interval_;
	util::Time expiration_;
	size_t iterationCount_;
	size_t iterationLimit_;


public:
	Timer(const util::DiffTime &interval, size_t iterationLimit = 0, const util::Time &t0 = util::Time::now())
	: interval_(interval)
	, expiration_(t0 + interval)
	, iterationCount_(0)
	, iterationLimit_(iterationLimit)
	{}

	bool operator<(const Timer &rhs) const { return expiration_ < rhs.expiration_; }

	const util::DiffTime &interval() const { return interval_; }
	const util::Time &expiration() const { return expiration_; }
	size_t iterationCount() const { return iterationCount_; }
	size_t iterationLimit() const { return iterationLimit_; }

	bool hasRemainingIterations() const;
	void fire();
};

#endif // REACTOR_TIMER_HEADER

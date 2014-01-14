#ifndef REACTOR_TIME_HEADER
#define REACTOR_TIME_HEADER

#include "DiffTime.hh"

class Time {
	uint64_t time_;

	explicit Time(uint64_t time) : time_(time) {}

public:
	static Time now();
	static Time raw(uint64_t time) { return Time(time); }

	Time() : time_(0) {}

	DiffTime operator-(const Time &rhs) const { return DiffTime::raw(time_ - rhs.time_); }

	Time operator+(const DiffTime &rhs) const { return Time(time_ + rhs.raw()); }
	Time &operator+=(const DiffTime &rhs) { time_ += rhs.raw(); return *this; }
	Time operator-(const DiffTime &rhs) const { return Time(time_ - rhs.raw()); }
	Time &operator-=(const DiffTime &rhs) { time_ -= rhs.raw(); return *this; }

	bool operator<(const Time &rhs) const { return time_ < rhs.time_; }

	uint64_t raw() const { return time_; }
//	time_t unixtime() const { return time_ >> 32; }
	uint32_t fraction(uint32_t multiplier) const { return ((time_ & 0xffffffff) * multiplier) >> 32; }
	uint32_t msFraction() const { return fraction(1000); }
	uint32_t usFraction() const { return fraction(1000 * 1000); }
	uint32_t nsFraction() const { return fraction(1000 * 1000 * 1000); }
};

#endif // REACTOR_TIME_HEADER

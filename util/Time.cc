#include "Time.hh"

#include "ErrnoException.hh"

#include <sys/time.h> // gettimeofday()

Time
Time::now()
{
	struct timeval tv;
	int ret = gettimeofday(&tv, 0);

	if (ret) {
		throw ErrnoException("gettimeofday");
	}

	uint64_t i = (uint64_t)tv.tv_sec << 32;
	uint64_t frac = (((uint64_t)tv.tv_usec << 32) + 999999) / 1000000;
	return Time(i | frac);
}

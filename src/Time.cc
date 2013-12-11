#include "Time.hh"

#include "ErrnoException.hh"

#include <sys/time.h> // gettimeofday()

Time
Time::now()
{
	struct timeval tv;
	int ret = gettimeofday(&tv, 0);
	if (ret) throw ErrnoException("gettimeofday");
	return Time(((uint64_t)tv.tv_sec << 32) | (((uint64_t)tv.tv_usec << 32) / 1000000));
}

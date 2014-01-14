#include "DiffTime.hh"

#include <cstdlib> // abs()

DiffTime
DiffTime::ms(int32_t ms)
{
	bool negative = ms < 0;
	int64_t ums = negative ? -ms : ms;
	int64_t i = (ums / 1000) << 32;
	int64_t frac = (((ums % 1000) << 32) + (negative ? 0 : 999)) / 1000;
	int64_t uresult = i | frac;
	return DiffTime(negative ? -uresult : uresult);
}

int32_t
DiffTime::ms() const
{
	int64_t i = raw_ >> 32;
	int64_t frac = ((raw_ & 0xffffffff) * 1000) >> 32;
	return i * 1000 + frac;
}

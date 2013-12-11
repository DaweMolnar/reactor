#include "DiffTime.hh"

DiffTime
DiffTime::ms(int32_t ms)
{
	int64_t i = (int64_t)(ms / 1000) << 32;
	int64_t frac = ((int64_t)(ms % 1000) << 32) / 1000;
	return DiffTime(i | frac);
}

int32_t
DiffTime::ms() const
{
	int64_t i = raw_ >> 32;
	int64_t frac = ((raw_ & 0xffffffff) * 1000) >> 32;
	return i * 1000 + frac;
}

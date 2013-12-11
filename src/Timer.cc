#include "Timer.hh"

bool
Timer::next()
{
	++counter_;
	if (counter_ == iterationLimit_) {
		return false;
	} else {
		expiration_ += interval_;
		return true;
	}
}

#include "Timer.hh"

#include <stdexcept>

using namespace reactor;

bool
Timer::hasRemainingIterations()
const
{
	return !iterationLimit_ || (iterationCount_ < iterationLimit_);
}

void
Timer::fire()
{
	if (!hasRemainingIterations()) {
		throw std::runtime_error("no more iterations");
	}
	expiration_ += interval_;
	++iterationCount_;
}

#include "Timers.hh"

void
Timers::add(const TimerAction &timerAction)
{
	queue_.push(timerAction);
}

bool
Timers::fireAllButUnexpired(DiffTime *remaining)
{
	for (int i = queue_.size(); !queue_.empty(); --i) {
		TimerAction ta(queue_.top());
		const DiffTime dt(ta.first.expiration() - Time::now());

		if (!dt.positive() && (i >= 0)) {
			queue_.pop();
			ta.second->perform();
			ta.first.fire();
			if (ta.first.hasRemainingIterations()) {
				queue_.push(ta);
			} else {
				guard_.release(ta.second);
			}
		} else {
			if (remaining) {
				*remaining = dt;
			}
			return true;
		}
	}
	return false;
}

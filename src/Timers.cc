#include "Timers.hh"

void
Timers::add(const Timer &timer, const Action &action)
{
	Action *actionCopy = guard_.copy(action);
	queue_.push(std::make_pair(timer, actionCopy));
}

bool
Timers::fireAllButUnexpired(DiffTime *remaining)
{
	for (int i = queue_.size(); !queue_.empty(); --i) {
		TimerAction ta(queue_.top());
		const DiffTime dt(ta.first.expiration() - nowFunc_());

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

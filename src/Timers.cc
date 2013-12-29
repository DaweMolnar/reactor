#include "Timers.hh"

void
Timers::add(const Timer &timer, const Action &action)
{
	Action *actionCopy = guard_.copy(action);
	queue_.push(TimerAction(timer, actionCopy));
}

bool
Timers::fireAllButUnexpired(DiffTime *remaining)
{
	for (int i = queue_.size(); !queue_.empty(); --i) {
		TimerAction ta(queue_.top());
		const DiffTime dt(ta.timer.expiration() - nowFunc_());

		if (!dt.positive() && (i >= 0)) {
			queue_.pop();
			ta.action->perform();
			ta.timer.fire();
			if (ta.timer.hasRemainingIterations()) {
				queue_.push(ta);
			} else {
				guard_.release(ta.action);
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

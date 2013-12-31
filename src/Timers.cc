#include "Timers.hh"

Timers::~Timers()
{
	while (!queue_.empty()) {
		delete queue_.top().command;
		queue_.pop();
	}
}

void
Timers::add(const Timer &timer, const TimerCommand &timerCommand)
{
	queue_.push(TimerAndCommand(timer, timerCommand.clone()));
}

bool
Timers::fireAllButUnexpired(DiffTime *remaining)
{
	for (int i = queue_.size(); !queue_.empty(); --i) {
		TimerAndCommand tac(queue_.top());
		const DiffTime dt(tac.timer.expiration() - nowFunc_());

		if (!dt.positive() && (i >= 0)) {
			queue_.pop();
			tac.command->execute(tac.timer);
			tac.timer.fire();
			if (tac.timer.hasRemainingIterations()) {
				queue_.push(tac);
			} else {
				delete tac.command;
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

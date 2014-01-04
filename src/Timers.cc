#include "Timers.hh"

#include <list>
#include <stdexcept>

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

void
Timers::harvest()
{
	typedef std::list<TimerAndCommand> Reinsertands;
	Reinsertands ri;

	while (!queue_.empty()) {
		TimerAndCommand tac(queue_.top());
		const DiffTime dt(tac.timer.expiration() - nowFunc_());

		if (!dt.positive()) {
			queue_.pop();
			backlog_.enqueueClone(bindCommand(*tac.command, TimerEvent(tac.timer)));
			tac.timer.fire();
			if (tac.timer.hasRemainingIterations()) {
				ri.push_back(tac);
			} else {
				delete tac.command;
			}
		} else {
			break;
		}
	}
	for (Reinsertands::const_iterator i(ri.begin()); i != ri.end(); ++i) {
		queue_.push(*i);
	}
}

bool
Timers::isTicking()
const
{
	return !queue_.empty();
}

DiffTime
Timers::remainingTime()
const
{
	if (queue_.empty()) {
		throw std::runtime_error("no ticking timer");
	}

	TimerAndCommand tac(queue_.top());
	DiffTime dt(tac.timer.expiration() - nowFunc_());
	return dt.positive() ? dt : DiffTime::raw(0);
}

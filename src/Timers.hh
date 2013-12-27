#ifndef REACTOR_TIMERS_HEADER
#define REACTOR_TIMERS_HEADER

#include "Noncopyable.hh"
#include "ActionsGuard.hh"
#include "LazyTimer.hh"

#include <queue>

class Timers : public Noncopyable {
	typedef std::pair<Timer, Action *> TimerAction;

	class TimerActionComparator : public std::less<TimerAction> {
	public:
		bool operator() (const TimerAction &a, const TimerAction &b) const { return !(a.first < b.first); }
	};
	typedef std::priority_queue<TimerAction, std::vector<TimerAction>, TimerActionComparator> Queue;

	Queue queue_;
	ActionsGuard &guard_;

public:
	Timers(ActionsGuard &guard)
	: guard_(guard)
	{}

	void add(const Timer &timer, const Action &action);
	bool fireAllButUnexpired(DiffTime *remaining = 0);
};

#endif // REACTOR_TIMERS_HEADER

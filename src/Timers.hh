#ifndef REACTOR_TIMERS_HEADER
#define REACTOR_TIMERS_HEADER

#include "Noncopyable.hh"
#include "ActionsGuard.hh"
#include "Timer.hh"

#include <queue>

class Timers : public Noncopyable {
public:
	typedef std::pair<Timer, Action *> TimerAction;

private:
	class TimerActionComparator : public std::less<TimerAction> {
	public:
		bool operator() (const TimerAction &a, const TimerAction &b) const { return a.first < b.first; }
	};
	typedef std::priority_queue<TimerAction, std::vector<TimerAction>, TimerActionComparator> Queue;

	Queue queue_;
	ActionsGuard &guard_;

public:
	Timers(ActionsGuard &guard)
	: guard_(guard)
	{}

	void add(const TimerAction &timerAction);
	bool fireAllButUnexpired(DiffTime *remaining = 0);
};

#endif // REACTOR_TIMERS_HEADER

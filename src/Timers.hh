#ifndef REACTOR_TIMERS_HEADER
#define REACTOR_TIMERS_HEADER

#include "Noncopyable.hh"
#include "ActionsGuard.hh"
#include "LazyTimer.hh"

#include <queue>

class Timers : public Noncopyable {
public:
	typedef Time (*NowFunc)();

private:
	struct TimerAction {
		Timer timer;
		Action *action;

		TimerAction(const Timer &timer0, Action *action0)
		: timer(timer0)
		, action(action0)
		{}
	};

	class TimerActionComparator : public std::less<TimerAction> {
	public:
		bool operator() (const TimerAction &a, const TimerAction &b) const { return !(a.timer < b.timer); }
	};
	typedef std::priority_queue<TimerAction, std::vector<TimerAction>, TimerActionComparator> Queue;

	Queue queue_;
	ActionsGuard &guard_;
	NowFunc nowFunc_;

public:
	Timers(ActionsGuard &guard, const NowFunc &nowFunc = Time::now)
	: guard_(guard)
	, nowFunc_(nowFunc)
	{}

	void add(const Timer &timer, const Action &action);
	bool fireAllButUnexpired(DiffTime *remaining = 0);
};

#endif // REACTOR_TIMERS_HEADER

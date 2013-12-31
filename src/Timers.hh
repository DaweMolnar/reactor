#ifndef REACTOR_TIMERS_HEADER
#define REACTOR_TIMERS_HEADER

#include "Noncopyable.hh"
#include "LazyTimer.hh"
#include "TimerCommand.hh"

#include <queue>

class Timers : public Noncopyable {
public:
	typedef Time (*NowFunc)();

private:
	struct TimerAndCommand {
		Timer timer;
		TimerCommand *command;

		TimerAndCommand(const Timer &timer0, TimerCommand *command0)
		: timer(timer0)
		, command(command0)
		{}
	};

	class TimerAndCommandComparator : public std::less<TimerAndCommand> {
	public:
		bool operator() (const TimerAndCommand &a, const TimerAndCommand &b) const { return !(a.timer < b.timer); }
	};
	typedef std::priority_queue<TimerAndCommand, std::vector<TimerAndCommand>, TimerAndCommandComparator> Queue;

	Queue queue_;
	NowFunc nowFunc_;

public:
	Timers(const NowFunc &nowFunc = Time::now)
	: nowFunc_(nowFunc)
	{}

	~Timers();

	void add(const Timer &timer, const TimerCommand &timerCommand);
	bool fireAllButUnexpired(DiffTime *remaining = 0);
};

#endif // REACTOR_TIMERS_HEADER

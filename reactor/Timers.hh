#ifndef REACTOR_REACTOR_TIMERS_HEADER
#define REACTOR_REACTOR_TIMERS_HEADER

#include <reactor/LazyTimer.hh>
#include <reactor/TimerCommand.hh>
#include <reactor/Backlog.hh>

#include <util/Noncopyable.hh>

#include <queue>

namespace reactor {

class Timers : public util::Noncopyable {
public:
	typedef util::Time (*NowFunc)();

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
	Backlog &backlog_;
	NowFunc nowFunc_;

public:
	Timers(Backlog &backlog, const NowFunc &nowFunc = util::Time::now)
	: backlog_(backlog)
	, nowFunc_(nowFunc)
	{}

	~Timers();

	void add(const Timer &timer, const TimerCommand &timerCommand);
	void harvest();
	bool isTicking() const;
	util::DiffTime remainingTime() const;
};

} // namespace reactor

#endif // REACTOR_REACTOR_TIMERS_HEADER

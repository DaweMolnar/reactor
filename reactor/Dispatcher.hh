#ifndef REACTOR_REACTOR_DISPATCHER_HEADER
#define REACTOR_REACTOR_DISPATCHER_HEADER

#include <reactor/Timers.hh>
#include <reactor/DefaultDemuxer.hh>
#include <reactor/FdCommand.hh>
#include <reactor/Backlog.hh>

#include <util/Pipe.hh>
#include <util/Noncopyable.hh>

#include <map>
#include <memory> // auto_ptr

namespace reactor {

class Dispatcher : public util::Noncopyable {
public:
	typedef Demuxer::FdEvents FdEvents;

private:
	typedef std::map<FdEvent, FdCommand *> FdCommands;

	FdCommands fdCommands_;
	Backlog backlog_;
	Timers timers_, lazyTimers_;
	std::auto_ptr<DefaultDemuxer> defaultDemuxer_;
	Demuxer *demuxer_;
	util::Pipe notifier_;

	void suspend(const FdEvent &fdEvent);
	void resume(const FdEvent &fdEvent);
	void lookupAndSchedule(FdEvent event);
	void collectFdEvents();
	void handleNotification(const FdEvent &event);

	friend class BoundResumingCommand;

protected:
	Dispatcher(Demuxer *demuxer = 0, const Timers::NowFunc nowFunc = util::Time::now);
	~Dispatcher();

public:
	static Dispatcher &instance();

	void collectEvents(FdEvents *fdEvents);
	bool hasPendingEvents() const;
	Backlog::Job *dequeueEvent();
	void stepSingleThread();
	util::DiffTime *remaining() const;
	FdEvents *wait(const util::DiffTime *remaining = 0) const;
	void notify();

	void add(const FdEvent &fdEvent, const FdCommand &command);
	void add(const Timer &timer, const TimerCommand &command);
	void add(const LazyTimer &lazyTimer, const TimerCommand &command);
};

} // namespace reactor

#endif // REACTOR_REACTOR_DISPATCHER_HEADER

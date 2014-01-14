#ifndef REACTOR_DISPATCHER_HEADER
#define REACTOR_DISPATCHER_HEADER

#include "Timers.hh"
#include "DefaultDemuxer.hh"
#include "FdCommand.hh"
#include "Backlog.hh"

#include <util/Pipe.hh>
#include <util/Noncopyable.hh>

#include <map>
#include <memory> // auto_ptr

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

public:
	Dispatcher(Demuxer *demuxer = 0, const Timers::NowFunc nowFunc = util::Time::now);
	~Dispatcher();

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

#endif // REACTOR_DISPATCHER_HEADER

#ifndef REACTOR_DISPATCHER_HEADER
#define REACTOR_DISPATCHER_HEADER

#include "Noncopyable.hh"
#include "Timers.hh"
#include "DefaultDemuxer.hh"
#include "FdCommand.hh"
#include "Backlog.hh"
#include "Pipe.hh"

#include <map>
#include <memory> // auto_ptr

class Dispatcher : public Noncopyable {
	typedef std::map<Fd, FdCommand *> FdCommands;

	FdCommands fdCommands_;
	Backlog backlog_;
	Timers timers_, lazyTimers_;
	std::auto_ptr<DefaultDemuxer> defaultDemuxer_;
	Demuxer *demuxer_;
	Pipe notifier_;

	void suspend(const Fd &fd);
	void resume(const Fd &fd);
	void lookupAndSchedule(FdEvent event);
	void collectFdEvents();
	void handleNotification(const FdEvent &event);

	friend class BoundResumingCommand;

public:
	Dispatcher(Demuxer *demuxer = 0, const Timers::NowFunc nowFunc = Time::now);
	~Dispatcher();

	void collectEvents();
	bool hasPendingEvents() const;
	Backlog::Job *dequeueEvent();
	void stepSingleThread();
	void notify();

	void add(const Fd &fd, const FdCommand &command);
	void add(const Timer &timer, const TimerCommand &command);
	void add(const LazyTimer &lazyTimer, const TimerCommand &command);
};

#endif // REACTOR_DISPATCHER_HEADER

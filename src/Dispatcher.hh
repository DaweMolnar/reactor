#ifndef REACTOR_DISPATCHER_HEADER
#define REACTOR_DISPATCHER_HEADER

#include "Noncopyable.hh"
#include "Timers.hh"
#include "DefaultDemuxer.hh"
#include "FdCommand.hh"
#include "Backlog.hh"

#include <map>
#include <memory> // auto_ptr

class Dispatcher : public Noncopyable {
	typedef std::map<int, FdCommand *> FdCommands; // XXX: map<Fd, FdCommand *>

	FdCommands fdCommands_;
	Backlog backlog_;
	Timers timers_, lazyTimers_;
	bool quit_;
	std::auto_ptr<DefaultDemuxer> defaultDemuxer_;
	Demuxer *demuxer_;

public:
	Dispatcher(Demuxer *demuxer = 0, const Timers::NowFunc nowFunc = Time::now)
	: timers_(backlog_, nowFunc)
	, lazyTimers_(backlog_, nowFunc)
	, quit_(false)
	, defaultDemuxer_(demuxer ? 0 : new DefaultDemuxer())
	, demuxer_(demuxer ? demuxer : defaultDemuxer_.get())
	{}

	~Dispatcher();

	void step();
	int run();
	void quit() { quit_ = true; }

	void add(const Fd &fd, const FdCommand &command);
	void add(const Timer &timer, const TimerCommand &command);
	void add(const LazyTimer &lazyTimer, const TimerCommand &command);
};

#endif // REACTOR_DISPATCHER_HEADER

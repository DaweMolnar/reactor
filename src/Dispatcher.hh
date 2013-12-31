#ifndef REACTOR_DISPATCHER_HEADER
#define REACTOR_DISPATCHER_HEADER

#include "Noncopyable.hh"
#include "ActionsGuard.hh"
#include "Timers.hh"
#include "DefaultDemuxer.hh"
#include "FdCommand.hh"

#include <map>
#include <memory> // auto_ptr

class Dispatcher : public Noncopyable {
	typedef std::map<int, FdCommand *> FdCommands; // XXX: map<Fd, FdCommand *>

	ActionsGuard guard_;
	FdCommands fdCommands_;
	Timers timers_, lazyTimers_;
	bool quit_;
	std::auto_ptr<DefaultDemuxer> defaultDemuxer_;
	Demuxer *demuxer_;

public:
	Dispatcher(Demuxer *demuxer = 0, const Timers::NowFunc nowFunc = Time::now)
	: timers_(guard_, nowFunc)
	, lazyTimers_(guard_, nowFunc)
	, quit_(false)
	, defaultDemuxer_(demuxer ? 0 : new DefaultDemuxer())
	, demuxer_(demuxer ? demuxer : defaultDemuxer_.get())
	{}

	~Dispatcher();

	void step();
	int run();
	void quit() { quit_ = true; }

	void add(const Fd &fd, const FdCommand &fdCommand);
	void add(const Timer &timer, const Action &action);
	void add(const LazyTimer &lazyTimer, const Action &action);
};

#endif // REACTOR_DISPATCHER_HEADER

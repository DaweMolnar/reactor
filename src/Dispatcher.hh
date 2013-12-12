#ifndef REACTOR_DISPATCHER_HEADER
#define REACTOR_DISPATCHER_HEADER

#include "Noncopyable.hh"
#include "ActionsGuard.hh"
#include "Timers.hh"
#include "DefaultDemuxer.hh"

#include <map>
#include <memory> // auto_ptr

class Dispatcher : public Noncopyable {
	typedef std::map<int, Action *> FdHandlers;

	ActionsGuard guard_;
	FdHandlers fdHandlers_;
	Timers timers_, lazyTimers_;
	bool quit_;
	std::auto_ptr<Demuxer> demuxer_;

public:
	Dispatcher(Demuxer *demuxer = 0)
	: timers_(guard_)
	, lazyTimers_(guard_)
	, quit_(false)
	, demuxer_(demuxer ? demuxer : new DefaultDemuxer())
	{}

	void step();
	int run();
	void quit() { quit_ = true; }

	void add(const Fd &fd, const Action &action);
	void add(const Timer &timer, const Action &action);
};

#endif // REACTOR_DISPATCHER_HEADER

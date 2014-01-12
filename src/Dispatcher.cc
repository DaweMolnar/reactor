#include "Dispatcher.hh"

#include <algorithm> // std::for_each
#include <stdexcept>

class BoundResumingCommand : public BoundCommand1<void, FdEvent, Command1<void, const FdEvent &> > {
	typedef Command1<void, const FdEvent &> C;
	typedef BoundCommand1<void, FdEvent, C> Base;

	Dispatcher &dispatcher_;

public:
	BoundResumingCommand(const C &command, const FdEvent &event, Dispatcher &dispatcher)
	: Base(command, event)
	, dispatcher_(dispatcher)
	{}

	~BoundResumingCommand()
	{
		if (own()) {
			dispatcher_.resume(Base::p1_);
		}
	}

	virtual BoundResumingCommand *
	clone()
	const
	{
		return new BoundResumingCommand(*this);
	}

	virtual void
	execute()
	const
	{
		Base::execute();
	}
};

Dispatcher::Dispatcher(Demuxer *demuxer, const Timers::NowFunc nowFunc)
: timers_(backlog_, nowFunc)
, lazyTimers_(backlog_, nowFunc)
, defaultDemuxer_(demuxer ? 0 : new DefaultDemuxer())
, demuxer_(demuxer ? demuxer : defaultDemuxer_.get())
{
	demuxer_->add(FdEvent(notifier_.readFd(), FdEvent::READ));
}

Dispatcher::~Dispatcher()
{
	for (FdCommands::const_iterator i(fdCommands_.begin()); i != fdCommands_.end(); ++i) {
		delete i->second;
	}
}

void
Dispatcher::add(const FdEvent &fdEvent, const FdCommand &command)
{
	demuxer_->add(fdEvent);
	fdCommands_.insert(std::make_pair(fdEvent, command.clone()));
}

void
Dispatcher::add(const Timer &timer, const TimerCommand &command)
{
	timers_.add(timer, command);
}

void
Dispatcher::add(const LazyTimer &lazyTimer, const TimerCommand &command)
{
	lazyTimers_.add(lazyTimer, command);
}

void
Dispatcher::suspend(const FdEvent &fdEvent)
{
	demuxer_->remove(fdEvent);
}

void
Dispatcher::resume(const FdEvent &fdEvent)
{
	demuxer_->add(fdEvent);
}

void
Dispatcher::lookupAndSchedule(FdEvent event)
{
	if (event.fd == notifier_.readFd()) {
		handleNotification(event);
		return;
	}

	FdCommands::iterator j(fdCommands_.find(event));

	if (j == fdCommands_.end()) {
		throw std::runtime_error("invalid fd");
	}

	suspend(event);
	backlog_.enqueueClone(BoundResumingCommand(*j->second, event, *this));
}

DiffTime *
Dispatcher::remaining()
const
{
	return timers_.isTicking() ? new DiffTime(timers_.remainingTime()) : 0;
}

Dispatcher::FdEvents *
Dispatcher::wait(const DiffTime *remaining)
const
{
	std::auto_ptr<const DiffTime> dt(remaining);
	return demuxer_->demux(dt.get());
}

void
Dispatcher::collectEvents(FdEvents *fdEvents)
{
	std::auto_ptr<FdEvents> e(fdEvents);
	std::for_each(e->begin(), e->end(), std::bind1st(std::mem_fun(&Dispatcher::lookupAndSchedule), this));
	timers_.harvest();
	lazyTimers_.harvest();
}

bool
Dispatcher::hasPendingEvents()
const
{
	return !backlog_.empty();
}

Backlog::Job *
Dispatcher::dequeueEvent()
{
	return backlog_.dequeue();
}

void
Dispatcher::stepSingleThread()
{
	collectEvents(wait(remaining()));
	while (hasPendingEvents()) {
		Backlog::Job *job = dequeueEvent();
		job->execute();
		delete job;
	}
}

void
Dispatcher::notify()
{
	notifier_.write("A", 1);
}

void
Dispatcher::handleNotification(const FdEvent &event)
{
	char dummy;
	event.fd.read(&dummy, sizeof(dummy));
}

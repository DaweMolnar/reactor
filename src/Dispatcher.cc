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
			dispatcher_.resume(Base::p1_.fd);
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
	demuxer_->add(notifier_.readFd());
}

Dispatcher::~Dispatcher()
{
	for (FdCommands::const_iterator i(fdCommands_.begin()); i != fdCommands_.end(); ++i) {
		delete i->second;
	}
}

void
Dispatcher::add(const Fd &fd, const FdCommand &command)
{
	demuxer_->add(fd);
	fdCommands_.insert(std::make_pair(fd, command.clone()));
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
Dispatcher::suspend(const Fd &fd)
{
	demuxer_->remove(fd);
}

void
Dispatcher::resume(const Fd &fd)
{
	demuxer_->add(fd);
}

void
Dispatcher::lookupAndSchedule(FdEvent event)
{
	if (event.fd == notifier_.readFd()) {
		handleNotification(event);
		return;
	}

	FdCommands::iterator j(fdCommands_.find(event.fd));

	if (j == fdCommands_.end()) {
		throw std::runtime_error("invalid fd");
	}

	suspend(event.fd);
	backlog_.enqueueClone(BoundResumingCommand(*j->second, event, *this));
}

void
Dispatcher::collectFdEvents()
{
	std::auto_ptr<DiffTime> remaining;

	if (timers_.isTicking()) {
		remaining.reset(new DiffTime(timers_.remainingTime()));
	}

	Demuxer::FdEvents fdEvs = demuxer_->demux(remaining.get());
	std::for_each(fdEvs.begin(), fdEvs.end(), std::bind1st(std::mem_fun(&Dispatcher::lookupAndSchedule), this));
}

void
Dispatcher::collectEvents()
{
	collectFdEvents();
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
	collectEvents();
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

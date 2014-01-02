#include "Dispatcher.hh"

#include <algorithm> // std::for_each
#include <stdexcept>
#include <cstdlib>

class BoundResumingCommand : public BoundCommand1<void, FdEvent, Command1<void, const FdEvent &> > {
	typedef Command1<void, const FdEvent &> C;
	typedef BoundCommand1<void, FdEvent, C> Base;

	Dispatcher &dispatcher_;

public:
	BoundResumingCommand(const C &command, const FdEvent &event, Dispatcher &dispatcher)
	: Base(command, event)
	, dispatcher_(dispatcher)
	{}

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
		dispatcher_.resume(Base::p1_.fd);
	}
};

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
	FdCommands::iterator j(fdCommands_.find(event.fd));

	if (j == fdCommands_.end()) {
		throw std::runtime_error("invalid fd");
	}

	suspend(event.fd);
	backlog_.push(BoundResumingCommand(*j->second, event, *this));
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
	timers_.scheduleAllExpired();
	lazyTimers_.scheduleAllExpired();
}

bool
Dispatcher::hasPendingEvent()
const
{
	return !backlog_.empty();
}

void
Dispatcher::processEvent()
{
	backlog_.executeFront();
}

void
Dispatcher::processAllEvents()
{
	backlog_.executeAll();
}

int
Dispatcher::run()
{
	while (!quit_) {
		collectEvents();
		processAllEvents();
	}
	return EXIT_SUCCESS;
}

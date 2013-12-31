#include "Dispatcher.hh"

#include <stdexcept>
#include <cstdlib>

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
	fdCommands_.insert(std::make_pair(fd.get(), command.clone()));
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
Dispatcher::step()
{
	DiffTime remaining;
	bool isTickingTimer = timers_.fireAllButUnexpired(&remaining);
	Demuxer::FdEvents fdEvs = demuxer_->demux(isTickingTimer ? &remaining : 0);
	for (Demuxer::FdEvents::const_iterator i(fdEvs.begin()); i != fdEvs.end(); ++i) {
		FdCommands::iterator j(fdCommands_.find(i->fd.get()));
		if (j == fdCommands_.end()) {
			throw std::runtime_error("invalid fd");
		} else {
			j->second->execute();
		}
	}
	lazyTimers_.fireAllButUnexpired();
}

int
Dispatcher::run()
{
	while (!quit_) step();
	return EXIT_SUCCESS;
}

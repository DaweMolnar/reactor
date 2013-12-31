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
Dispatcher::add(const Fd &fd, const FdCommand &fdCommand)
{
	demuxer_->add(fd);
	fdCommands_.insert(std::make_pair(fd.get(), fdCommand.clone()));
}

void
Dispatcher::add(const Timer &timer, const Action &action)
{
	timers_.add(timer, action);
}

void
Dispatcher::add(const LazyTimer &lazyTimer, const Action &action)
{
	lazyTimers_.add(lazyTimer, action);
}

void
Dispatcher::step()
{
	DiffTime remaining;
	bool isTickingTimer = timers_.fireAllButUnexpired(&remaining);
	Demuxer::Fds fds = demuxer_->demux(isTickingTimer ? &remaining : 0);
	for (Demuxer::Fds::const_iterator i(fds.begin()); i != fds.end(); ++i) {
		FdCommands::iterator j(fdCommands_.find(*i));
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

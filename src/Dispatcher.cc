#include "Dispatcher.hh"

#include <stdexcept>
#include <cstdlib>

void
Dispatcher::add(const Fd &fd, const Action &action)
{
	Action *a = guard_.copy(action);
	demuxer_->add(fd);
	fdHandlers_.insert(std::make_pair(fd.get(), a));
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
		FdHandlers::iterator j(fdHandlers_.find(*i));
		if (j == fdHandlers_.end()) {
			throw std::runtime_error("invalid fd");
		} else {
			j->second->perform();
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

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
	Action *a = guard_.copy(action);
	Timers &timers(timer.lazy() ? lazyTimers_ : timers_);
	timers.add(std::make_pair(timer, a));
}

void
Dispatcher::step()
{
	DiffTime remaining;
	bool isTickingTimer = timers_.fireAllButUnexpired(&remaining);
	std::vector<int> fds;

	demuxer_->demux(isTickingTimer ? &remaining : 0, fds);
	for (size_t i = 0; i < fds.size(); ++i) {
		FdHandlers::iterator j(fdHandlers_.find(fds[i]));
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

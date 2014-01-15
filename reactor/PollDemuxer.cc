#include "PollDemuxer.hh"

#include <util/ErrnoException.hh>

void
PollDemuxer::add(const FdEvent &fdEvent)
{
	if (fdEvent.fd.valid()) {
		struct pollfd pfd;

		pfd.fd = fdEvent.fd.get();
		pfd.events = 0;
		if (fdEvent.what == FdEvent::READ) {
			pfd.events |= POLLIN;
		}
		if (fdEvent.what == FdEvent::WRITE) {
			pfd.events |= POLLOUT;
		}
		pfd.revents = 0;

		fds_.push_back(pfd);
	}
}

void
PollDemuxer::remove(const FdEvent &fdEvent)
{
	// XXX: we can possibly do better than linear search
	for (Fds::iterator i(fds_.begin()); i != fds_.end(); ++i) {
		if (i->fd == fdEvent.fd.get()) {
			if (((i->events & POLLIN) == POLLIN) && (fdEvent.what == FdEvent::READ)) {
				fds_.erase(i);
				break;
			}
			if (((i->events & POLLOUT) == POLLOUT) && (fdEvent.what == FdEvent::WRITE)) {
				fds_.erase(i);
				break;
			}
		}
	}
}

PollDemuxer::FdEvents *
PollDemuxer::demux(const util::DiffTime *interval)
{
	int ms = interval ? interval->ms() : -1;
	int ret = poll(&fds_[0], fds_.size(), ms);

	if (ret < 0) {
		throw util::ErrnoException("poll");
	} else {
		FdEvents *result = new FdEvents();

		for (size_t i = 0; i < fds_.size(); ++i) {
			if ((fds_[i].revents & POLLIN) == POLLIN) {
				result->push_back(FdEvent(util::Fd(fds_[i].fd), FdEvent::READ));
			}
			if ((fds_[i].revents & POLLOUT) == POLLOUT) {
				result->push_back(FdEvent(util::Fd(fds_[i].fd), FdEvent::WRITE));
			}
		}

		return result;
	}
}

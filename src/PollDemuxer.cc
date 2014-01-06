#include "PollDemuxer.hh"

#include "ErrnoException.hh"

void
PollDemuxer::add(const Fd &fd)
{
	if (fd.valid()) {
		struct pollfd pfd;

		pfd.fd = fd.get();
		pfd.events = POLLIN;
		pfd.revents = 0;

		fds_.push_back(pfd);
	}
}

void
PollDemuxer::remove(const Fd &fd)
{
	// XXX: we can possibly do better than linear search
	for (Fds::iterator i(fds_.begin()); i != fds_.end(); ++i) {
		if (i->fd == fd.get()) {
			fds_.erase(i);
			break;
		}
	}
}

PollDemuxer::FdEvents *
PollDemuxer::demux(const DiffTime *interval)
{
	int ms = interval ? interval->ms() : -1;
	int ret = poll(&fds_[0], fds_.size(), ms);

	if (ret < 0) {
		throw ErrnoException("poll");
	} else {
		FdEvents *result = new FdEvents();

		for (size_t i = 0; i < fds_.size(); ++i) {
			if ((fds_[i].revents & POLLIN) == POLLIN) {
				result->push_back(FdEvent(Fd(fds_[i].fd), FdEvent::READ));
			}
			if ((fds_[i].revents & POLLOUT) == POLLOUT) {
				result->push_back(FdEvent(Fd(fds_[i].fd), FdEvent::WRITE));
			}
		}

		return result;
	}
}

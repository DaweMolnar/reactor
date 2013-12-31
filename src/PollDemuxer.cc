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

PollDemuxer::Fds
PollDemuxer::demux(const DiffTime *interval)
{
	int ms = interval ? interval->ms() : -1;
	int ret = poll(&fds_[0], fds_.size(), ms);

	if (ret < 0) {
		throw ErrnoException("poll");
	} else {
		Fds result;

		for (size_t i = 0; i < fds_.size(); ++i) {
			if (fds_[i].revents) {
				result.push_back(fds_[i].fd);
			}
		}

		return result;
	}
}


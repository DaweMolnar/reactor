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
PollDemuxer::demux(const DiffTime *interval, Fds &fds)
{
	int ms = interval ? interval->ms() : -1;
	int ret = poll(&fds_[0], fds_.size(), ms);

	if (ret < 0) {
		throw ErrnoException("poll");
	} else {
		fds.resize(ret);
		if (ret > 0) {
			size_t k = 0;

			for (size_t i = 0; i < fds_.size(); ++i) {
				if (fds_[i].revents) {
					fds[k++] = fds_[i].fd;
				}
			}
		}
	}
}


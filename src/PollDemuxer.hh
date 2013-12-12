#ifndef REACTOR_POLLDEMUXER_HEADER
#define REACTOR_POLLDEMUXER_HEADER

#include "Demuxer.hh"

#include <poll.h>

class PollDemuxer : public Demuxer {
	std::vector<struct pollfd> fds_;

public:
	virtual void add(const Fd &fd);
	virtual void demux(const DiffTime *interval, std::vector<int> &fds);
};

#endif // REACTOR_POLLDEMUXER_HEADER

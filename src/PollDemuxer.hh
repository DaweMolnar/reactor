#ifndef REACTOR_POLLDEMUXER_HEADER
#define REACTOR_POLLDEMUXER_HEADER

#include "Demuxer.hh"

#include <poll.h>
#include <vector>

class PollDemuxer : public Demuxer {
	typedef std::vector<struct pollfd> Fds;

	Fds fds_;

public:
	virtual void add(const FdEvent &fdEvent);
	virtual void remove(const FdEvent &fdEvent);
	virtual FdEvents *demux(const DiffTime *interval);
};

#endif // REACTOR_POLLDEMUXER_HEADER

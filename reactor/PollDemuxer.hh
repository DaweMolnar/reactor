#ifndef REACTOR_REACTOR_POLLDEMUXER_HEADER
#define REACTOR_REACTOR_POLLDEMUXER_HEADER

#include <reactor/Demuxer.hh>

#include <poll.h>
#include <vector>

class PollDemuxer : public Demuxer {
	typedef std::vector<struct pollfd> Fds;

	Fds fds_;

public:
	virtual void add(const FdEvent &fdEvent);
	virtual void remove(const FdEvent &fdEvent);
	virtual FdEvents *demux(const util::DiffTime *interval);
};

#endif // REACTOR_REACTOR_POLLDEMUXER_HEADER

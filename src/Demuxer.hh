#ifndef REACTOR_DEMUXER_HEADER
#define REACTOR_DEMUXER_HEADER

#include "Noncopyable.hh"
#include "Fd.hh"
#include "DiffTime.hh"

#include <vector>

class Demuxer : public Noncopyable {
public:
	typedef std::vector<int> Fds;

	virtual ~Demuxer() {}

	virtual void add(const Fd &fd) = 0;
	virtual void demux(const DiffTime *interval, Fds &fds) = 0;
};

#endif // REACTOR_DEMUXER_HEADER

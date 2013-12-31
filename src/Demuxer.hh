#ifndef REACTOR_DEMUXER_HEADER
#define REACTOR_DEMUXER_HEADER

#include "Noncopyable.hh"
#include "Fd.hh"
#include "DiffTime.hh"

#include <list>

class Demuxer : public Noncopyable {
public:
	typedef std::list<int> Fds;

	virtual ~Demuxer() {}

	virtual void add(const Fd &fd) = 0;
	virtual Fds demux(const DiffTime *interval) = 0;
};

#endif // REACTOR_DEMUXER_HEADER

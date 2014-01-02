#ifndef REACTOR_DEMUXER_HEADER
#define REACTOR_DEMUXER_HEADER

#include "Noncopyable.hh"
#include "Fd.hh"
#include "DiffTime.hh"
#include "FdEvent.hh"

#include <list>

class Demuxer : public Noncopyable {
public:
	typedef std::list<FdEvent> FdEvents;

	virtual ~Demuxer() {}

	virtual void add(const Fd &fd) = 0;
	virtual void remove(const Fd &fd) = 0;
	virtual FdEvents demux(const DiffTime *interval) = 0;
};

#endif // REACTOR_DEMUXER_HEADER

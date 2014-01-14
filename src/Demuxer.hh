#ifndef REACTOR_DEMUXER_HEADER
#define REACTOR_DEMUXER_HEADER

#include "FdEvent.hh"

#include <util/DiffTime.hh>
#include <util/Fd.hh>
#include <util/Noncopyable.hh>

#include <list>

class Demuxer : public util::Noncopyable {
public:
	typedef std::list<FdEvent> FdEvents;

	virtual ~Demuxer() {}

	virtual void add(const FdEvent &fdEvent) = 0;
	virtual void remove(const FdEvent &fdEvent) = 0;
	virtual FdEvents *demux(const util::DiffTime *interval) = 0;
};

#endif // REACTOR_DEMUXER_HEADER

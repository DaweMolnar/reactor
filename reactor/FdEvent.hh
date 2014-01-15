#ifndef REACTOR_REACTOR_FDEVENT_HEADER
#define REACTOR_REACTOR_FDEVENT_HEADER

#include <util/Fd.hh>

namespace reactor {

struct FdEvent {
	enum What {
		READ = 4,
		WRITE = 2
	};

	util::Fd fd;
	What what;

	FdEvent(const util::Fd &fd0, What what0)
	: fd(fd0)
	, what(what0)
	{}

	bool
	operator<(const FdEvent &rhs)
	const
	{
		if (fd != rhs.fd) {
			return fd < rhs.fd;
		} else {
			return what < rhs.what;
		}
	}
};

} // namespace reactor

#endif // REACTOR_REACTOR_FDEVENT_HEADER

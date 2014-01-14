#ifndef REACTOR_FDEVENT_HEADER
#define REACTOR_FDEVENT_HEADER

#include <util/Fd.hh>

struct FdEvent {
	enum What {
		READ = 4,
		WRITE = 2
	};

	Fd fd;
	What what;

	FdEvent(const Fd &fd0, What what0)
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

#endif // REACTOR_FDEVENT_HEADER

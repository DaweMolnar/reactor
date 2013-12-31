#ifndef REACTOR_FDEVENT_HEADER
#define REACTOR_FDEVENT_HEADER

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
};

#endif // REACTOR_FDEVENT_HEADER

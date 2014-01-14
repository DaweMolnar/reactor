#ifndef REACTOR_PIPE_HEADER
#define REACTOR_PIPE_HEADER

#include "AutoFd.hh"

class Pipe {
	AutoFd readFd_;
	AutoFd writeFd_;

public:
	Pipe();

	const Fd &readFd() const { return readFd_; }

	size_t write(const void *buffer, size_t length) const;
};

#endif // REACTOR_PIPE_HEADER

#ifndef REACTOR_UTIL_PIPE_HEADER
#define REACTOR_UTIL_PIPE_HEADER

#include <util/AutoFd.hh>

namespace util {

class Pipe {
	AutoFd readFd_;
	AutoFd writeFd_;

public:
	Pipe();

	const Fd &readFd() const { return readFd_; }

	size_t write(const void *buffer, size_t length) const;
};

} // namespace util

#endif // REACTOR_UTIL_PIPE_HEADER

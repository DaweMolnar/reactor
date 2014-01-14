#ifndef REACTOR_UTIL_AUTO_FD_HEADER
#define REACTOR_UTIL_AUTO_FD_HEADER

#include <util/Fd.hh>
#include <util/Noncopyable.hh>

namespace util {

class AutoFd : public Fd, public Noncopyable {
	void closeIfValid();

public:
	explicit AutoFd(int fd = INVALID) : Fd(fd) {}
	~AutoFd();

	int release();
	void reset(int fd = INVALID);
};

} // namespace util

#endif // REACTOR_UTIL_AUTO_FD_HEADER

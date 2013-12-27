#ifndef REACTOR_AUTO_FD_HEADER
#define REACTOR_AUTO_FD_HEADER

#include "Fd.hh"

class AutoFd : public Fd {
	void closeIfValid();

public:
	explicit AutoFd(int fd = INVALID) : Fd(fd) {}
	~AutoFd();

	int release();
	void reset(int fd = INVALID);
};

#endif // REACTOR_AUTO_FD_HEADER

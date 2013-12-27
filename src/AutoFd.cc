#include "AutoFd.hh"

#include <unistd.h>

AutoFd::~AutoFd()
{
	closeIfValid();
}

void
AutoFd::closeIfValid()
{
	if (valid()) {
		close(fd_);
	}
}

int
AutoFd::release()
{
	int fd = fd_;

	fd_ = INVALID;

	return fd;
}

void
AutoFd::reset(int fd)
{
	closeIfValid();
	fd_ = fd;
}

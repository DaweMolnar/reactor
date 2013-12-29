#include "AutoFd.hh"

AutoFd::~AutoFd()
{
	closeIfValid();
}

void
AutoFd::closeIfValid()
{
	if (valid()) {
		close();
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

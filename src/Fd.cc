#include "Fd.hh"

#include "ErrnoException.hh"

#include <fcntl.h> // blocking
#include <unistd.h>

const Fd Fd::STDIN(STDIN_FILENO);
const Fd Fd::STDOUT(STDOUT_FILENO);
const Fd Fd::STDERR(STDERR_FILENO);

int
Fd::release(void)
{
	int fd = fd_;

	if (valid()) {
		close(fd_);
		fd_ = INVALID;
	}

	return fd;
}

size_t
Fd::read(void *buffer, size_t size)
const
{
	ssize_t ret = ::read(get(), buffer, size);

	if (ret < 0) {
		throw ErrnoException("read");
	}
	return (size_t)ret;
}

size_t
Fd::write(const void *buffer, size_t length)
const
{
	ssize_t ret = ::write(get(), buffer, length);

	if (ret < 0) {
		throw ErrnoException("write");
	}
	return (size_t)ret;
}

bool
Fd::blocking()
const
{
	int ret = fcntl(get(), F_GETFL, 0);
	if (ret < 0) {
		throw ErrnoException("fcntl");
	}
	return !(ret & O_NONBLOCK);
}

void
Fd::blocking(bool block)
{
	int flags = fcntl(get(), F_GETFL, 0);

	if (flags < 0) {
		throw ErrnoException("fcntl");
	} else {
		bool oldBlock = !(flags & O_NONBLOCK);

		if (block != oldBlock) {
			flags ^= O_NONBLOCK;
			if (fcntl(get(), F_SETFL, flags) == -1) {
				throw ErrnoException("fcntl");
			}
		}
	}
}

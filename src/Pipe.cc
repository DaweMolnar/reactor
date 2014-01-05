#include "Pipe.hh"

#include "ErrnoException.hh"

#include <unistd.h>

Pipe::Pipe()
{
	int fds[2];

	if (pipe(fds)) {
		throw ErrnoException("pipe");
	}

	readFd_.reset(fds[0]);
	writeFd_.reset(fds[1]);
}

size_t
Pipe::write(const void *buffer, size_t length)
const
{
	return writeFd_.write(buffer, length);
}

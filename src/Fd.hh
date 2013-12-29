#ifndef REACTOR_FD_HEADER
#define REACTOR_FD_HEADER

#include "Noncopyable.hh"

#include <cstddef>

class Fd : public Noncopyable {
protected:
	int fd_;

public:
	enum { INVALID = -1 };

	static const Fd STDIN;
	static const Fd STDOUT;
	static const Fd STDERR;

	explicit Fd(int fd = INVALID) : fd_(fd) {}

	bool valid() const { return fd_ != INVALID; }

	int get() const { return fd_; }

	size_t read(void *buffer, size_t size) const;
	size_t write(const void *buffer, size_t length) const;

	void close();

	bool blocking() const;
	void blocking(bool block);
};

#endif // REACTOR_FD_HEADER

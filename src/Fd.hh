#ifndef REACTOR_FD_HEADER
#define REACTOR_FD_HEADER

#include "Noncopyable.hh"

class Fd : public Noncopyable {
	int fd_;

public:
	enum { INVALID = -1 };

	static const Fd STDIN;
	static const Fd STDOUT;
	static const Fd STDERR;

	explicit Fd(int fd = INVALID) : fd_(fd) {}
	~Fd() { release(); }

	bool valid() const { return fd_ != INVALID; }

	int get() const { return fd_; }
	int release();
	void reset(int fd = INVALID) { release(); fd_ = fd; }

	size_t read(void *buffer, size_t size) const;
	size_t write(const void *buffer, size_t length) const;

	bool blocking() const;
	void blocking(bool block);
};

#endif // REACTOR_FD_HEADER

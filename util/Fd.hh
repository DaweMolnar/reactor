#ifndef REACTOR_UTIL_FD_HEADER
#define REACTOR_UTIL_FD_HEADER

#include <cstddef>

namespace util {

class Fd {
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

	bool operator<(const Fd &rhs) const { return fd_ < rhs.fd_; }
	bool operator==(const Fd &rhs) const { return fd_ == rhs.fd_; }
	bool operator!=(const Fd &rhs) const { return fd_ != rhs.fd_; }
};

} // namespace util

#endif // REACTOR_UTIL_FD_HEADER

#ifdef _GNU_SOURCE
#undef _GNU_SOURCE
#endif
#include <string.h> // strerror_r()
#ifdef _GNU_SOURCE
#error "Failed to undef _GNU_SOURCE"
#endif

#include "ErrnoException.hh"

#include <sstream>
#include <cerrno>

ErrnoException::ErrnoException(const std::string &name)
: errno_(errno)
, name_(name)
{
	std::ostringstream oss;
	char buf[256];
	int error = strerror_r(errno_, buf, sizeof(buf));

	oss << name_ << ": ";
	if (!error) {
		oss << buf;
	} else {
		oss << "[strerror_r() failure]";
	}
	oss << " (" << errno_ << ")";
	what_ = oss.str();
}
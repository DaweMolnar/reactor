#include <string.h> // strerror()

#include "ErrnoException.hh"

#include <sstream>
#include <cerrno>

using namespace util;

ErrnoException::ErrnoException(const std::string &name, int errorCode)
: errorCode_(errorCode ? errorCode : errno)
, name_(name)
{
	std::ostringstream oss;
	oss << name_ << ": " << strerror(errorCode_);
	oss << " (" << errorCode_ << ")";
	what_ = oss.str();
}

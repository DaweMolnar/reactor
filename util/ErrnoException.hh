#ifndef REACTOR_UTIL_ERRNOEXCEPTION_HEADER
#define REACTOR_UTIL_ERRNOEXCEPTION_HEADER

#include <exception>
#include <string>

namespace util {

class ErrnoException : public std::exception {
	int errorCode_;
	const std::string name_;
	std::string what_;

public:
	explicit ErrnoException(const std::string &name, int errorCode = 0);
	virtual ~ErrnoException() throw() {}

	int errorCode() const { return errorCode_; }
	const std::string &name() const { return name_; }
	virtual const char *what() const throw() { return what_.c_str(); }
};

} // namespace util

#endif // REACTOR_UTIL_ERRNOEXCEPTION_HEADER

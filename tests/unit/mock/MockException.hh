#ifndef REACTOR_MOCK_EXCEPTION_HEADER
#define REACTOR_MOCK_EXCEPTION_HEADER

#include <stdexcept>

class MockException : public std::runtime_error {
public:
	MockException(const std::string &name, const std::string &reason)
	: std::runtime_error(name + ": " + reason)
	{}
};

#endif // REACTOR_MOCK_EXCEPTION_HEADER

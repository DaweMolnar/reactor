#ifndef REACTOR_NET_SPECIFIER_HEADER
#define REACTOR_NET_SPECIFIER_HEADER

#include <string>

namespace net {

class Specifier {
	std::string spec_;
	int aiFlags_;

public:
	Specifier(const std::string &spec = "", int aiFlags = 0)
	: spec_(spec)
	, aiFlags_(aiFlags)
	{}

	const std::string &spec() const { return spec_; }
	int aiFlags() const { return aiFlags_; }
};

} // namespace net

#endif // REACTOR_NET_SPECIFIER_HEADER

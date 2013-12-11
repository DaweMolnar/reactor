#ifndef REACTOR_SPECIFIER_HEADER
#define REACTOR_SPECIFIER_HEADER

#include <string>

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

#endif // REACTOR_SPECIFIER_HEADER

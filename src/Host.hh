#ifndef REACTOR_HOST_HEADER
#define REACTOR_HOST_HEADER

#include "Specifier.hh"

class Host : public Specifier {
public:
	static const int NUMERIC;

	Host(const std::string &spec = "", int aiFlags = 0)
	: Specifier(spec, aiFlags)
	{}
};

#endif // REACTOR_HOST_HEADER

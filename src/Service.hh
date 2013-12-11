#ifndef REACTOR_SERVICE_HEADER
#define REACTOR_SERVICE_HEADER

#include "Specifier.hh"

class Service : public Specifier {
public:
	static const int NUMERIC;

	explicit Service(const std::string &spec = "", int aiFlags = 0)
	: Specifier(spec, aiFlags)
	{}
};

#endif // REACTOR_SERVICE_HEADER

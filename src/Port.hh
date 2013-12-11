#ifndef REACTOR_PORT_HEADER
#define REACTOR_PORT_HEADER

#include "Service.hh"

class Port : public Service {
public:
	Port(const std::string &port = "", int aiFlags = 0)
	: Service(port, aiFlags | NUMERIC)
	{}
};

#endif // REACTOR_PORT_HEADER

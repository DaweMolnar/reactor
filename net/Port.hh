#ifndef REACTOR_NET_PORT_HEADER
#define REACTOR_NET_PORT_HEADER

#include <net/Service.hh>

#include <stdint.h>

namespace net {

class Port : public Service {
	static std::string makeValidPort(uint16_t port);

public:
	Port(uint16_t port, int aiFlags = 0)
	: Service(makeValidPort(port), aiFlags | NUMERIC)
	{}
};

} // namespace net

#endif // REACTOR_NET_PORT_HEADER

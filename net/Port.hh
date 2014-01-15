#ifndef REACTOR_NET_PORT_HEADER
#define REACTOR_NET_PORT_HEADER

#include <net/Service.hh>

namespace net {

class Port : public Service {
public:
	Port(const std::string &port = "", int aiFlags = 0)
	: Service(port, aiFlags | NUMERIC)
	{}
};

} // namespace net

#endif // REACTOR_NET_PORT_HEADER

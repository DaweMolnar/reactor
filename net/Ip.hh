#ifndef REACTOR_NET_IP_HEADER
#define REACTOR_NET_IP_HEADER

#include <net/Host.hh>

namespace net {

class Ip : public Host {
public:
	static const Ip ANY;

	explicit Ip(const std::string &ip = "", int aiFlags = 0)
	: Host(ip, aiFlags | NUMERIC)
	{}
};

} // namespace net

#endif // REACTOR_NET_IP_HEADER

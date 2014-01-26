#ifndef REACTOR_NET_IP_HEADER
#define REACTOR_NET_IP_HEADER

#include <net/Host.hh>

namespace net {

class Ip : public Host {
	static std::string makeValidIp(const std::string &ip);

public:
	static const Ip ANY;

	Ip(const std::string &ip, int aiFlags = 0)
	: Host(makeValidIp(ip), aiFlags | NUMERIC)
	{}

	Ip(const char *ip, int aiFlags = 0)
	: Host(makeValidIp(ip), aiFlags | NUMERIC)
	{}
};

} // namespace net

#endif // REACTOR_NET_IP_HEADER

#ifndef REACTOR_IP_HEADER
#define REACTOR_IP_HEADER

#include "Host.hh"

class Ip : public Host {
public:
	static const Ip ANY;

	explicit Ip(const std::string &ip = "", int aiFlags = 0)
	: Host(ip, aiFlags | NUMERIC)
	{}
};

#endif // REACTOR_IP_HEADER

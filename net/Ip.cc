#include "Ip.hh"

#include <arpa/inet.h>

#include <stdexcept>

using namespace net;

const Ip Ip::ANY("0.0.0.0");

std::string
Ip::makeValidIp(const std::string &ip)
{
	in_addr inAddr;
	in6_addr in6Addr;

	if (inet_pton(AF_INET, ip.c_str(), &inAddr) == 1) {
		return ip;
	} else if (inet_pton(AF_INET6, ip.c_str(), &in6Addr) == 1) {
		return ip;
	} else {
		throw std::invalid_argument(std::string("invalid IP address: \"") + ip + "\"");
	}
}

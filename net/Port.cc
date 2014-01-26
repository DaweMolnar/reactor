#include "Port.hh"

#include <sstream>

using namespace net;

std::string
Port::makeValidPort(uint16_t port)
{
	std::ostringstream result;

	result << port;

	return result.str();
}

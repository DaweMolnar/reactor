#include "Client.hh"

using namespace reactor;

void
Client::setTarget(const net::Host &targetHost, const net::Service &targetServ)
{
	targetHost_ = targetHost;
	targetServ_ = targetServ;
}

void
Client::connect()
{
	sock_.connect(targetHost_, targetServ_);
}

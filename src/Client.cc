#include "Client.hh"

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

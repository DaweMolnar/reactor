#ifndef REACTOR_CLIENT_HEADER
#define REACTOR_CLIENT_HEADER

#include "Dispatcher.hh"
#include "Host.hh"
#include "Service.hh"
#include "StreamSock.hh"

#include <util/Noncopyable.hh>

class Client : public Noncopyable {
	Dispatcher &dispatcher_;
	Host targetHost_;
	Service targetServ_;
	Host sourceHost_;
	StreamSock sock_;

public:
	Client(Dispatcher &dispatcher)
	: dispatcher_(dispatcher)
	{}

	void setTarget(const Host &targetHost, const Service &targetServ);
	void connect();

	const Fd &fd() const { return sock_.fd(); }
};

#endif // REACTOR_CLIENT_HEADER

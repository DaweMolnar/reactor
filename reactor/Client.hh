#ifndef REACTOR_REACTOR_CLIENT_HEADER
#define REACTOR_REACTOR_CLIENT_HEADER

#include <reactor/Dispatcher.hh>
#include <reactor/StreamSock.hh>

#include <net/Host.hh>
#include <net/Service.hh>
#include <util/Noncopyable.hh>

class Client : public util::Noncopyable {
	Dispatcher &dispatcher_;
	net::Host targetHost_;
	net::Service targetServ_;
	net::Host sourceHost_;
	StreamSock sock_;

public:
	Client(Dispatcher &dispatcher)
	: dispatcher_(dispatcher)
	{}

	void setTarget(const net::Host &targetHost, const net::Service &targetServ);
	void connect();

	const util::Fd &fd() const { return sock_.fd(); }
};

#endif // REACTOR_REACTOR_CLIENT_HEADER

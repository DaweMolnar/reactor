#ifndef REACTOR_SOCKET_HEADER
#define REACTOR_SOCKET_HEADER

#include "Host.hh"
#include "Service.hh"

#include <util/AutoFd.hh>
#include <util/Noncopyable.hh>

class Socket : public Noncopyable {
	AutoFd fd_;
	int type_;

public:
	static const int ANY;
	static const int STREAM;
	static const int DGRAM;

	Socket(int type) : type_(type) {}

	void connect(const Host &targetHost, const Service &targetServ);
	const Fd &fd() const { return fd_; }
};

#endif // REACTOR_SOCKET_HEADER

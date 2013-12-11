#ifndef REACTOR_SOCKET_HEADER
#define REACTOR_SOCKET_HEADER

#include "Noncopyable.hh"
#include "Fd.hh"
#include "Host.hh"
#include "Service.hh"

class Socket : public Noncopyable {
	Fd fd_;
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

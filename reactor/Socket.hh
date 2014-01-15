#ifndef REACTOR_REACTOR_SOCKET_HEADER
#define REACTOR_REACTOR_SOCKET_HEADER

#include <net/Host.hh>
#include <net/Service.hh>
#include <util/AutoFd.hh>
#include <util/Noncopyable.hh>

namespace reactor {

class Socket : public util::Noncopyable {
	util::AutoFd fd_;
	int type_;

public:
	static const int ANY;
	static const int STREAM;
	static const int DGRAM;

	Socket(int type) : type_(type) {}

	void connect(const net::Host &targetHost, const net::Service &targetServ);
	const util::Fd &fd() const { return fd_; }
};

} // namespace reactor

#endif // REACTOR_REACTOR_SOCKET_HEADER

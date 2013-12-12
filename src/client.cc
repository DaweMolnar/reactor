#include "ErrnoException.hh"
#include "StreamSock.hh"
#include "ActionMethod.hh"
#include "LazyTimer.hh"
#include "Timers.hh"
#include "PollDemuxer.hh"
#include "Dispatcher.hh"
#include "Client.hh"

#include <stdint.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <stdexcept>
#include <vector>
#include <map>
#include <sstream>
#include <set>

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))

void
Client::setTarget(const Host &targetHost, const Service &targetServ)
{
	targetHost_ = targetHost;
	targetServ_ = targetServ;
}

void
Client::connect()
{
	sock_.connect(targetHost_, targetServ_);
}

class Control {
	Dispatcher dispatcher_;
	Client client_;

public:
	Control(int argc, char *argv[]);

	void onFdStdin();
	void onFdSock();
	void onTimer();
	int run() { return dispatcher_.run(); }
};

Control::Control(int argc, char *argv[])
: client_(dispatcher_)
{
	if (argc != 3) throw std::runtime_error("argc must be 3");
//	client_.setTarget(Ip("127.0.0.1"), Port("8080"));
	client_.setTarget(Host(argv[1]), Service(argv[2]));
	client_.connect();

	dispatcher_.add(Fd::STDIN, genActionMethod(*this, &Control::onFdStdin));
	dispatcher_.add(client_.fd(), genActionMethod(*this, &Control::onFdSock));
	dispatcher_.add(Timer(DiffTime::ms(1000), 3), genActionMethod(*this, &Control::onTimer));
}

void
Control::onFdStdin()
{
	char buf[128];
	size_t rd = Fd::STDIN.read(buf, sizeof(buf));

	if (!rd) {
		dispatcher_.quit();
	} else {
		size_t wr = client_.fd().write(buf, rd);

		if (wr != rd) {
			throw std::runtime_error("partial send");
		}
	}
}

void
Control::onFdSock()
{
	char buf[128];
	size_t rd = client_.fd().read(buf, sizeof(buf));

	if (!rd) {
		dispatcher_.quit();
	} else {
		size_t wr = Fd::STDOUT.write(buf, rd);

		if (wr != rd) {
			throw std::runtime_error("partial send");
		}
	}
}

void
Control::onTimer()
{
	fprintf(stderr, "timer\n");
}

int
main(int argc, char *argv[])
{
	return Control(argc, argv).run();
}

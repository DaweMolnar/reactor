#include "Client.hh"

#include <stdexcept>

class Control {
	Dispatcher dispatcher_;
	Client client_;

public:
	Control(int argc, char *argv[]);

	void onFdStdin(const FdEvent &);
	void onFdSock(const FdEvent &);
	void onTimer(const TimerEvent &);
	int run() { return dispatcher_.run(); }
};

Control::Control(int argc, char *argv[])
: client_(dispatcher_)
{
	if (argc != 3) throw std::runtime_error("argc must be 3");
//	client_.setTarget(Ip("127.0.0.1"), Port("8080"));
	client_.setTarget(Host(argv[1]), Service(argv[2]));
	client_.connect();

	dispatcher_.add(Fd::STDIN, commandForMethod(*this, &Control::onFdStdin));
	dispatcher_.add(client_.fd(), commandForMethod(*this, &Control::onFdSock));
	dispatcher_.add(Timer(DiffTime::ms(1000), 3), commandForMethod(*this, &Control::onTimer));
}

void
Control::onFdStdin(const FdEvent &event)
{
	char buf[128];
	size_t rd = event.fd.read(buf, sizeof(buf));

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
Control::onFdSock(const FdEvent &event)
{
	char buf[128];
	size_t rd = event.fd.read(buf, sizeof(buf));

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
Control::onTimer(const TimerEvent &)
{
	Fd::STDERR.write("timer\n", 6);
}

int
main(int argc, char *argv[])
{
	return Control(argc, argv).run();
}

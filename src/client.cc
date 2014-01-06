#include "Client.hh"

#include "MultiReactor.hh"

#include <stdexcept>
#include <iostream>
#include <cstdlib>

class Main {
	Dispatcher dispatcher_;
	MultiReactor reactor_;
	Client client_;

public:
	Main(int argc, char *argv[]);

	void onFdStdin(const FdEvent &);
	void onFdSock(const FdEvent &);
	void onTimer(const TimerEvent &);
	int run();
};

Main::Main(int argc, char *argv[])
: reactor_(dispatcher_, 2)
, client_(dispatcher_)
{
	if (argc != 3) throw std::runtime_error("argc must be 3");
//	client_.setTarget(Ip("127.0.0.1"), Port("8080"));
	std::cerr << "resolving names..." << std::endl;
	client_.setTarget(Host(argv[1]), Service(argv[2]));
	std::cerr << "connecting..." << std::endl;
	client_.connect();
	std::cerr << "connected." << std::endl;

	dispatcher_.add(Fd::STDIN, commandForMethod(*this, &Main::onFdStdin));
	dispatcher_.add(client_.fd(), commandForMethod(*this, &Main::onFdSock));
	dispatcher_.add(Timer(DiffTime::ms(1000), 3), commandForMethod(*this, &Main::onTimer));
}

int
Main::run()
{
	try {
		return reactor_.loop();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}

void
Main::onFdStdin(const FdEvent &event)
{
	char buf[128];
	size_t rd = event.fd.read(buf, sizeof(buf));

	if (!rd) {
		reactor_.quit();
	} else {
		size_t wr = client_.fd().write(buf, rd);

		if (wr != rd) {
			throw std::runtime_error("partial send");
		}
	}
}

void
Main::onFdSock(const FdEvent &event)
{
	char buf[128];
	size_t rd = event.fd.read(buf, sizeof(buf));

	if (!rd) {
		reactor_.quit();
	} else {
		size_t wr = Fd::STDOUT.write(buf, rd);

		if (wr != rd) {
			throw std::runtime_error("partial send");
		}
	}
}

void
Main::onTimer(const TimerEvent &)
{
	Fd::STDERR.write("timer\n", 6);
//	for (int i = 0; i < 1000000000; ++i);
//	Fd::STDERR.write("timer done\n", 11);
}

int
main(int argc, char *argv[])
{
	return Main(argc, argv).run();
}

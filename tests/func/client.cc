#include "FunctionalTest.hh"

#include <reactor/Client.hh>
#include <reactor/MultiReactor.hh>

#include <stdexcept>
#include <iostream>
#include <cstdlib>

using namespace reactor;

class ClientTester : public FunctionalTest {
	Dispatcher dispatcher_;
	MultiReactor reactor_;
	Client client_;

public:
	ClientTester(int argc, char *argv[]);

	void onFdStdin(const FdEvent &);
	void onFdSock(const FdEvent &);
	void onTimer(const TimerEvent &);
	virtual int run();
};

ClientTester::ClientTester(int argc, char *argv[])
: reactor_(dispatcher_, 2)
, client_(dispatcher_)
{
	if (argc != 3) throw std::runtime_error("argc must be 3");
//	client_.setTarget(Ip("127.0.0.1"), Port("8080"));
	std::cerr << "resolving names..." << std::endl;
	client_.setTarget(net::Host(argv[1]), net::Service(argv[2]));
	std::cerr << "connecting..." << std::endl;
	client_.connect();
	std::cerr << "connected." << std::endl;

	dispatcher_.add(FdEvent(util::Fd::STDIN, FdEvent::READ), util::commandForMethod(*this, &ClientTester::onFdStdin));
	dispatcher_.add(FdEvent(client_.fd(), FdEvent::READ), util::commandForMethod(*this, &ClientTester::onFdSock));
	dispatcher_.add(Timer(util::DiffTime::ms(1000), 3), util::commandForMethod(*this, &ClientTester::onTimer));
}

int
ClientTester::run()
{
	try {
		return reactor_.loop();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}

void
ClientTester::onFdStdin(const FdEvent &event)
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
ClientTester::onFdSock(const FdEvent &event)
{
	char buf[128];
	size_t rd = event.fd.read(buf, sizeof(buf));

	if (!rd) {
		reactor_.quit();
	} else {
		size_t wr = util::Fd::STDOUT.write(buf, rd);

		if (wr != rd) {
			throw std::runtime_error("partial send");
		}
	}
}

void
ClientTester::onTimer(const TimerEvent &)
{
	util::Fd::STDERR.write("timer\n", 6);
//	for (int i = 0; i < 1000000000; ++i);
//	Fd::STDERR.write("timer done\n", 11);
}

REGISTER_FUNCTIONAL_TEST(ClientTester);

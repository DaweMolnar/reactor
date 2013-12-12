#include "ErrnoException.hh"
#include "StreamSock.hh"
#include "ActionMethod.hh"
#include "LazyTimer.hh"
#include "Timers.hh"
#include "PollDemuxer.hh"

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
#include <memory> // auto_ptr

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))

typedef PollDemuxer DefaultDemuxer;

void
PollDemuxer::add(const Fd &fd)
{
	if (fd.valid()) {
		struct pollfd pfd;

		pfd.fd = fd.get();
		pfd.events = POLLIN;
		pfd.revents = 0;

		fds_.push_back(pfd);
	}
}

void
PollDemuxer::demux(const DiffTime *interval, std::vector<int> &fds)
{
	int ms = interval ? interval->ms() : -1;
	int ret = poll(&fds_[0], fds_.size(), ms);

	if (ret < 0) {
		throw ErrnoException("poll");
	} else {
		fds.resize(ret);
		if (ret > 0) {
			size_t k = 0;

			for (size_t i = 0; i < fds_.size(); ++i) {
				if (fds_[i].revents) {
					fds[k++] = fds_[i].fd;
				}
			}
		}
	}
}

class Dispatcher : public Noncopyable {
	typedef std::map<int, Action *> FdHandlers;

	ActionsGuard guard_;
	FdHandlers fdHandlers_;
	Timers timers_, lazyTimers_;
	bool quit_;
	std::auto_ptr<Demuxer> demuxer_;

public:
	Dispatcher(Demuxer *demuxer = 0)
	: timers_(guard_)
	, lazyTimers_(guard_)
	, quit_(false)
	, demuxer_(demuxer ? demuxer : new DefaultDemuxer())
	{}

	void step();
	int run();
	void quit() { quit_ = true; }

	void add(const Fd &fd, const Action &action);
	void add(const Timer &timer, const Action &action);
};

void
Dispatcher::add(const Fd &fd, const Action &action)
{
	Action *a = guard_.copy(action);
	demuxer_->add(fd);
	fdHandlers_.insert(std::make_pair(fd.get(), a));
}

void
Dispatcher::add(const Timer &timer, const Action &action)
{
	Action *a = guard_.copy(action);
	Timers &timers(timer.lazy() ? lazyTimers_ : timers_);
	timers.add(std::make_pair(timer, a));
}

void
Dispatcher::step()
{
	DiffTime remaining;
	bool isTickingTimer = timers_.fireAllButUnexpired(&remaining);
	std::vector<int> fds;

	demuxer_->demux(isTickingTimer ? &remaining : 0, fds);
	for (size_t i = 0; i < fds.size(); ++i) {
		FdHandlers::iterator j(fdHandlers_.find(fds[i]));
		if (j == fdHandlers_.end()) {
			throw std::runtime_error("invalid fd");
		} else {
			j->second->perform();
		}
	}
	lazyTimers_.fireAllButUnexpired();
}

int
Dispatcher::run()
{
	while (!quit_) step();
	return EXIT_SUCCESS;
}

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

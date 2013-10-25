#include <netdb.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <poll.h>
#include <string>
#include <stdexcept>
#include <vector>
#include <cerrno>
#include <map>

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))

class Noncopyable {
	Noncopyable(const Noncopyable &);
	Noncopyable &operator=(const Noncopyable &);

public:
	Noncopyable(void) {}
};

class Fd : public Noncopyable {
	int fd_;

public:
	enum { INVALID = -1 };

	static const Fd STDIN;
	static const Fd STDOUT;
	static const Fd STDERR;

	explicit Fd(int fd = INVALID) : fd_(fd) {}
	~Fd() { release(); }

	bool valid() const { return fd_ != INVALID; }

	int get() const { return fd_; }
	int release();
	void reset(int fd = INVALID) { release(); fd_ = fd; }

	size_t read(void *buffer, size_t size) const;
	size_t write(const void *buffer, size_t length) const;
};

const Fd Fd::STDIN(STDIN_FILENO);
const Fd Fd::STDOUT(STDOUT_FILENO);
const Fd Fd::STDERR(STDERR_FILENO);

int
Fd::release(void)
{
	int fd = fd_;

	if (valid()) {
		close(fd_);
		fd_ = INVALID;
	}

	return fd;
}

size_t
Fd::read(void *buffer, size_t size)
const
{
	ssize_t ret = ::read(get(), buffer, size);

	if (ret < 0) throw std::runtime_error("read");
	return (size_t)ret;
}

size_t
Fd::write(const void *buffer, size_t length)
const
{
	ssize_t ret = ::write(get(), buffer, length);

	if (ret < 0) throw std::runtime_error("write");
	return (size_t)ret;
}

class Specifier {
	std::string spec_;
	int aiFlags_;

public:
	Specifier(const std::string &spec = "", int aiFlags = 0)
	: spec_(spec)
	, aiFlags_(aiFlags)
	{}

	const std::string &spec() const { return spec_; }
	int aiFlags() const { return aiFlags_; }
};

class HostSpec : public Specifier {
public:
	static const int NUMERIC;

	HostSpec(const std::string &spec = "", int aiFlags = 0)
	: Specifier(spec, aiFlags)
	{}
};

const int HostSpec::NUMERIC = AI_NUMERICHOST;

class Ip : public HostSpec {
public:
	static const Ip ANY;

	explicit Ip(const std::string &ip = "", int aiFlags = 0)
	: HostSpec(ip, aiFlags | NUMERIC)
	{}
};

const Ip Ip::ANY("");

class ServSpec : public Specifier {
public:
	static const int NUMERIC;

	explicit ServSpec(const std::string &spec = "", int aiFlags = 0)
	: Specifier(spec, aiFlags)
	{}
};

const int ServSpec::NUMERIC = AI_NUMERICSERV;

class Port : public ServSpec {
public:
	Port(const std::string &port = "", int aiFlags = 0)
	: ServSpec(port, aiFlags | NUMERIC)
	{}
};

class Socket {
	Fd fd_;
	int type_;

public:
	static const int ANY;
	static const int STREAM;
	static const int DGRAM;

	Socket(int type) : type_(type) {}

	void connect(const HostSpec &targetHost, const ServSpec &targetServ);
	const Fd &fd() const { return fd_; }
};

const int Socket::ANY = 0;
const int Socket::STREAM = SOCK_STREAM;
const int Socket::DGRAM = SOCK_DGRAM;

void
Socket::connect(const HostSpec &targetHost, const ServSpec &targetServ)
{
	int ret;
	struct addrinfo hints, *res;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = type_;
	hints.ai_flags = targetHost.aiFlags() | targetServ.aiFlags();

	ret = getaddrinfo(targetHost.spec().c_str(), targetServ.spec().c_str(), &hints, &res);
	if (ret) {
		throw std::runtime_error(gai_strerror(ret));
	}

	for (struct addrinfo *p = res; p; p = p->ai_next) {
		fd_.reset(socket(p->ai_family, p->ai_socktype, p->ai_protocol));
		if (!fd_.valid()) continue;

		ret = ::connect(fd_.get(), p->ai_addr, p->ai_addrlen);
		if (ret != -1) break;

		fd_.release();
	}

	freeaddrinfo(res);

	if (!fd_.valid()) {
		throw std::runtime_error("failed to connect");
	}
}

class StreamSock : public Socket {
public:
	StreamSock() : Socket(STREAM) {}
};

class DgramSock : public Socket {
public:
	DgramSock() : Socket(DGRAM) {}
};

class Action {
public:
	virtual ~Action() {}

	virtual void perform() = 0;
};

template <class T>
class ActionMethod : public Action {
public:
	typedef void (T::*Method)();

private:
	T &object_;
	Method method_;

public:
	ActionMethod(T &object, Method method)
	: object_(object)
	, method_(method)
	{}

	virtual void perform() { (object_.*method_)(); }
};

class Poller : public Noncopyable {
	typedef std::map<int, Action *> FdHandlers;

	std::vector<struct pollfd> fds_;
	FdHandlers fdHandlers_;
	bool quit_;

public:
	Poller() : quit_(false) {}
	~Poller();

	int run();
	void quit() { quit_ = true; }
	void add(const Fd &fd);

	template <class T>
	void
	add(const Fd &fd, T &object, void (T::*method)())
	{
		add(fd);
		fdHandlers_.insert(std::make_pair(fd.get(), new ActionMethod<T>(object, method)));
	}
};

Poller::~Poller()
{
	for (FdHandlers::iterator i(fdHandlers_.begin()); i != fdHandlers_.end(); ++i) {
		delete i->second;
	}
	fdHandlers_.clear();
}

int
Poller::run(void)
{
	while (!quit_) {
		int ret = poll(&fds_[0], fds_.size(), -1);
		if (ret < 0) {
			throw std::runtime_error(strerror(errno));
		}
		for (size_t i = 0; i < fds_.size(); ++i) {
			if (fds_[i].revents) {
				FdHandlers::iterator j(fdHandlers_.find(fds_[i].fd));
				if (j != fdHandlers_.end()) {
					j->second->perform();
				}
			}
		}
	}
	return EXIT_SUCCESS;
}

void
Poller::add(const Fd &fd)
{
	if (fd.valid()) {
		struct pollfd pfd;

		pfd.fd = fd.get();
		pfd.events = POLLIN;
		pfd.revents = 0;

		fds_.push_back(pfd);
	}
}

class Client : public Noncopyable {
	Poller &poller_;
	HostSpec targetHost_;
	ServSpec targetServ_;
	HostSpec sourceHost_;
	StreamSock sock_;

public:
	Client(Poller &poller)
	: poller_(poller)
	{}

	void setTarget(const HostSpec &targetHost, const ServSpec &targetServ);
	void connect();

	const Fd &fd() const { return sock_.fd(); }
};

void
Client::setTarget(const HostSpec &targetHost, const ServSpec &targetServ)
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
	Poller poller_;
	Client client_;

public:
	Control();

	void onFdStdin();
	void onFdSock();
	int run() { return poller_.run(); }
};

Control::Control()
: client_(poller_)
{
	client_.setTarget(Ip("127.0.0.1"), Port("8080"));
	client_.connect();

	poller_.add(Fd::STDIN, *this, &Control::onFdStdin);
	poller_.add(client_.fd(), *this, &Control::onFdSock);
}

void
Control::onFdStdin()
{
	char buf[128];
	size_t rd = Fd::STDIN.read(buf, sizeof(buf));

	if (!rd) {
		poller_.quit();
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
		poller_.quit();
	} else {
		size_t wr = Fd::STDOUT.write(buf, rd);

		if (wr != rd) {
			throw std::runtime_error("partial send");
		}
	}
}

int
main()
{
	return Control().run();
}

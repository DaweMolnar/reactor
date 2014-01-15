#ifndef REACTOR_NET_SERVICE_HEADER
#define REACTOR_NET_SERVICE_HEADER

#include <net/Specifier.hh>

namespace net {

class Service : public Specifier {
public:
	static const int NUMERIC;

	explicit Service(const std::string &spec = "", int aiFlags = 0)
	: Specifier(spec, aiFlags)
	{}
};

} // namespace net

#endif // REACTOR_NET_SERVICE_HEADER

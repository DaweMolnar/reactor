#ifndef REACTOR_NET_HOST_HEADER
#define REACTOR_NET_HOST_HEADER

#include <net/Specifier.hh>

namespace net {

class Host : public Specifier {
public:
	static const int NUMERIC;

	Host(const std::string &spec = "", int aiFlags = 0)
	: Specifier(spec, aiFlags)
	{}
};

} // namespace net

#endif // REACTOR_NET_HOST_HEADER

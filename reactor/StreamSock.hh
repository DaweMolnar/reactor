#ifndef REACTOR_REACTOR_STREAMSOCK_HEADER
#define REACTOR_REACTOR_STREAMSOCK_HEADER

#include <reactor/Socket.hh>

namespace reactor {

class StreamSock : public Socket {
public:
	StreamSock() : Socket(STREAM) {}
};

} // namespace reactor

#endif // REACTOR_REACTOR_STREAMSOCK_HEADER

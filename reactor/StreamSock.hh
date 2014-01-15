#ifndef REACTOR_REACTOR_STREAMSOCK_HEADER
#define REACTOR_REACTOR_STREAMSOCK_HEADER

#include <reactor/Socket.hh>

class StreamSock : public Socket {
public:
	StreamSock() : Socket(STREAM) {}
};

#endif // REACTOR_REACTOR_STREAMSOCK_HEADER

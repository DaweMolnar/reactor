#ifndef REACTOR_STREAMSOCK_HEADER
#define REACTOR_STREAMSOCK_HEADER

#include "Socket.hh"

class StreamSock : public Socket {
public:
	StreamSock() : Socket(STREAM) {}
};

#endif // REACTOR_STREAMSOCK_HEADER

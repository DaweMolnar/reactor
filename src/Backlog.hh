#ifndef REACTOR_BACKLOG_HEADER
#define REACTOR_BACKLOG_HEADER

#include <util/BoundCommand.hh>

#include <queue>
#include <cstddef>

class Backlog {
public:
	typedef Command0<void> Job;

private:
	typedef std::queue<Job *> Queue;

	Queue queue_;

public:
	~Backlog();

	void enqueueClone(const Job &job);
	Job *dequeue();
	bool empty() const;
};

#endif // REACTOR_BACKLOG_HEADER

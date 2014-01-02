#ifndef REACTOR_BACKLOG_HEADER
#define REACTOR_BACKLOG_HEADER

#include "BoundCommand.hh"

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

	void push(const Job &job);
	void executeOne();
	void executeAll();
	bool empty() const;
};

#endif // REACTOR_BACKLOG_HEADER

#ifndef REACTOR_REACTOR_BACKLOG_HEADER
#define REACTOR_REACTOR_BACKLOG_HEADER

#include <util/BoundCommand.hh>

#include <queue>
#include <cstddef>

namespace reactor {

class Backlog {
public:
	typedef util::Command0<void> Job;

private:
	typedef std::queue<Job *> Queue;

	Queue queue_;

public:
	~Backlog();

	void enqueueClone(const Job &job);
	Job *dequeue();
	bool empty() const;
};

} // namespace reactor

#endif // REACTOR_REACTOR_BACKLOG_HEADER

#include "Backlog.hh"

#include <stdexcept>

using namespace reactor;

Backlog::~Backlog()
{
	while (!queue_.empty()) {
		delete queue_.front();
		queue_.pop();
	}
}

void
Backlog::enqueueClone(const Job &job)
{
	queue_.push(job.clone());
}

Backlog::Job *
Backlog::dequeue()
{
	if (!queue_.empty()) {
		Job *job = queue_.front();
		queue_.pop();

		return job;
	} else {
		throw std::runtime_error("no jobs in backlog to execute");
	}
}

bool
Backlog::empty()
const
{
	return queue_.empty();
}

#include "Backlog.hh"

#include <stdexcept>

Backlog::~Backlog()
{
	while (!queue_.empty()) {
		delete queue_.front();
		queue_.pop();
	}
}

void
Backlog::push(const Job &job)
{
	queue_.push(job.clone());
}

void
Backlog::executeFront()
{
	if (!queue_.empty()) {
		Job *job = queue_.front();
		queue_.pop();

		job->execute();
		delete job;
	} else {
		throw std::runtime_error("no jobs in backlog to execute");
	}
}

void
Backlog::executeAll()
{
	while (!queue_.empty()) {
		executeFront();
	}
}

bool
Backlog::empty()
const
{
	return queue_.empty();
}

size_t
Backlog::size()
const
{
	return queue_.size();
}

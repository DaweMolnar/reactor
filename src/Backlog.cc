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
Backlog::executeOne()
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
		executeOne();
	}
}

bool
Backlog::empty()
const
{
	return queue_.empty();
}

#include <src/ThreadCondition.hh>
#include <src/Thread.hh>
#include <src/Guard.hh>

#include <cppunit/extensions/HelperMacros.h>

#include <cstdio>

#define DEBUG(s) fprintf(stderr, "%s\n", s)

class ThreadConditionTester
: public CppUnit::TestFixture
, public Runnable
{
	CPPUNIT_TEST_SUITE(ThreadConditionTester);
	CPPUNIT_TEST(testNotify);
	CPPUNIT_TEST_SUITE_END();

	ThreadMutex *mutex_;
	ThreadCondition *cond_;

	enum {
		STATE_WAITING_THREAD,
		STATE_THREAD_OK,
		STATE_FINISH_REQUESTED,
		STATE_FINISHED
	} state_;

public:
	void
	setUp()
	{
		mutex_ = new ThreadMutex();
		cond_ = new ThreadCondition(*mutex_);
	}

	void
	tearDown()
	{
		delete cond_;
		delete mutex_;
	}

	virtual void
	run()
	{
		Guard<ThreadMutex> guard(*mutex_);
		CPPUNIT_ASSERT_EQUAL(STATE_WAITING_THREAD, state_);
		state_ = STATE_THREAD_OK;
		cond_->notify();
		while (state_ != STATE_FINISH_REQUESTED) {
			cond_->wait();
		}
		state_ = STATE_FINISHED;
		cond_->notify();
	}

	void
	testNotify()
	{
		state_ = STATE_WAITING_THREAD;
		Thread th(*this);

		{
			Guard<ThreadMutex> guard(*mutex_);
			while (state_ != STATE_THREAD_OK) {
				cond_->wait();
			}
			CPPUNIT_ASSERT_EQUAL(STATE_THREAD_OK, state_);
			cond_->notify();
			state_ = STATE_FINISH_REQUESTED;
			cond_->notify();
			while (state_ != STATE_FINISHED) {
				cond_->wait();
			}
			CPPUNIT_ASSERT_EQUAL(STATE_FINISHED, state_);
		}

		while (!th.hasFinished());
		CPPUNIT_ASSERT_EQUAL(true, th.hasFinished());
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(ThreadConditionTester);

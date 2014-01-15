#include <thread/Condition.hh>
#include <thread/Thread.hh>
#include <thread/Guard.hh>

#include <cppunit/extensions/HelperMacros.h>

#include <cstdio>

#define DEBUG(s) fprintf(stderr, "%s\n", s)

using namespace thread;

class ThreadConditionTester
: public CppUnit::TestFixture
, public util::Runnable
{
	CPPUNIT_TEST_SUITE(ThreadConditionTester);
	CPPUNIT_TEST(testNotify);
	CPPUNIT_TEST_SUITE_END();

	Mutex *mutex_;
	Condition *cond_;

	enum State {
		STATE_WAITING_THREAD,
		STATE_THREAD_OK,
		STATE_FINISH_REQUESTED,
		STATE_FINISHED
	};

	State state_;

public:
	void
	setUp()
	{
		mutex_ = new Mutex();
		cond_ = new Condition(*mutex_);
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
		Guard<Mutex> guard(*mutex_);
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
			Guard<Mutex> guard(*mutex_);
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

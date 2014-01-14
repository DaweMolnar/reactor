#include <thread/ThreadPool.hh>
#include <thread/ThreadMutex.hh>
#include <thread/Guard.hh>

#include <cppunit/extensions/HelperMacros.h>

class ThreadMutexTester
: public CppUnit::TestFixture
, public util::Runnable
{
	CPPUNIT_TEST_SUITE(ThreadMutexTester);
//	CPPUNIT_TEST(testThreadPoolWithNoLocking);
	CPPUNIT_TEST(testThreadPoolWithLocking);
	CPPUNIT_TEST_SUITE_END();

	static const size_t ITERATION_COUNT = 10000;
	static const size_t THREAD_COUNT = 10;

	ThreadMutex *tm_;
	size_t count_;
	bool useLocking_;

public:
	void
	setUp()
	{
		tm_ = new ThreadMutex();
		count_ = 0;
		useLocking_ = false;
	}

	void
	tearDown()
	{
		delete tm_;
	}

	virtual void
	run()
	{
		if (useLocking_) {
			for (size_t i = 0; i < ITERATION_COUNT; ++i) {
				Guard<ThreadMutex> guard(*tm_);
				++count_;
			}
		} else {
			for (size_t i = 0; i < ITERATION_COUNT; ++i) {
				++count_;
			}
		}
	}

	void
	runThreadPoolTest(bool useLocking)
	{
		useLocking_ = useLocking;
		count_ = 0;
		ThreadPool tp(*this, THREAD_COUNT);
	}

	void
	testThreadPoolWithNoLocking()
	{
		runThreadPoolTest(false);
		CPPUNIT_ASSERT(THREAD_COUNT * ITERATION_COUNT != count_);
	}

	void
	testThreadPoolWithLocking()
	{
		runThreadPoolTest(true);
		CPPUNIT_ASSERT_EQUAL(THREAD_COUNT * ITERATION_COUNT, count_);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(ThreadMutexTester);

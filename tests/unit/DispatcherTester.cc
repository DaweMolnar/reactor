#include <src/Dispatcher.hh>
#include <src/ActionMethod.hh>

#include <tests/unit/mock/Mocked.hh>
#include <tests/unit/mock/MockRegistry.hh>

#include <cppunit/extensions/HelperMacros.h>

#include <stdexcept>

class MyDemuxer : public Demuxer {
public:
	virtual void add(const Fd &fd);
	virtual Fds demux(const DiffTime *interval);
};

void
MyDemuxer::add(const Fd &fd)
{
	(void)fd;
}

MyDemuxer::Fds
MyDemuxer::demux(const DiffTime *interval)
{
	Fds result;
	Mocked &m = MockRegistry::find("demux");

	(void)interval;

	for (int i = m.expectedInt(); i; --i) {
		result.push_back(m.expectedInt());
	}

	return result;
}

class DispatcherTester : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(DispatcherTester);
	CPPUNIT_TEST(testFdAction);
	CPPUNIT_TEST(testInvalidFdAction);
	CPPUNIT_TEST(testRun);
	CPPUNIT_TEST(testTimerAction);
	CPPUNIT_TEST(testLazyTimerAction);
	CPPUNIT_TEST_SUITE_END();

	const ActionMethod<DispatcherTester> actionMethod_;
	MyDemuxer *dmx_;
	Dispatcher *disp_;
	size_t actionCount_;

public:
	DispatcherTester()
	: actionMethod_(ActionMethod<DispatcherTester>(*this, &DispatcherTester::action))
	{}

	void
	setUp()
	{
		dmx_ = new MyDemuxer();
		disp_ = new Dispatcher(dmx_, &DispatcherTester::now);
		actionCount_ = 0;
	}

	void
	tearDown()
	{
		delete disp_;
		delete dmx_;
	}

	void
	action()
	{
		++actionCount_;
		disp_->quit();
	}

	void
	testFdAction()
	{
		Mocked demux("demux");
		Fd fd(42);

		disp_->add(fd, actionMethod_);
		demux.expectf("%d%d", 1, 42);
		disp_->step();
		CPPUNIT_ASSERT_EQUAL((size_t)1, actionCount_);
	}

	void
	testInvalidFdAction()
	{
		Mocked demux("demux");
		Fd fd(43);

		disp_->add(fd, actionMethod_);
		demux.expectf("%d%d", 1, 42);
		CPPUNIT_ASSERT_THROW(disp_->step(), std::runtime_error);
	}

	void
	testRun()
	{
		Mocked demux("demux");
		Fd fd(42);

		disp_->add(fd, actionMethod_);
		demux.expectf("%d%d", 1, 42);
		CPPUNIT_ASSERT_NO_THROW(disp_->run());
	}

	static Time
	now()
	{
		Mocked &m = MockRegistry::find("now");
		return Time::raw(m.expectedInt());
	}

	void
	testTimerAction()
	{
		Mocked demux("demux");
		Mocked now("now");
		Timer t(DiffTime::raw(2), 0, Time::raw(0));

		disp_->add(t, actionMethod_);
		demux.expect(0);
		now.expect(2);
		now.expect(2);
		CPPUNIT_ASSERT_NO_THROW(disp_->run());
		CPPUNIT_ASSERT_EQUAL((size_t)1, actionCount_);
	}

	void
	testLazyTimerAction()
	{
		Mocked demux("demux");
		Mocked now("now");
		Fd fd(42);
		LazyTimer lt(DiffTime::raw(2), 0, Time::raw(0));

		disp_->add(fd, actionMethod_);
		disp_->add(lt, actionMethod_);
		demux.expectf("%d%d", 1, 42);
		now.expect(2);
		now.expect(2);
		disp_->step();
		CPPUNIT_ASSERT_EQUAL((size_t)2, actionCount_);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(DispatcherTester);

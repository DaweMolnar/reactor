#include <reactor/Dispatcher.hh>

#include <tests/unit/mock/Mocked.hh>
#include <tests/unit/mock/MockRegistry.hh>

#include <cppunit/extensions/HelperMacros.h>

#include <stdexcept>

using namespace util;

class MyDemuxer : public Demuxer {
public:
	virtual void add(const FdEvent &fdEvent);
	virtual void remove(const FdEvent &fdEvent);
	virtual FdEvents *demux(const DiffTime *interval);
};

void
MyDemuxer::add(const FdEvent &fdEvent)
{
	(void)fdEvent;
}

void
MyDemuxer::remove(const FdEvent &fdEvent)
{
	(void)fdEvent;
}

MyDemuxer::FdEvents *
MyDemuxer::demux(const DiffTime *interval)
{
	FdEvents *result = new FdEvents();
	Mocked &m = MockRegistry::find("demux");

	(void)interval;

	for (int i = m.expectedInt(); i; --i) {
		result->push_back(FdEvent(Fd(m.expectedInt()), FdEvent::READ));
	}

	return result;
}

class DispatcherTester : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(DispatcherTester);
	CPPUNIT_TEST(testFdAction);
	CPPUNIT_TEST(testInvalidFdAction);
	CPPUNIT_TEST(testTimerAction);
	CPPUNIT_TEST(testLazyTimerAction);
	CPPUNIT_TEST_SUITE_END();

	const MethodCommand1<void, DispatcherTester, const FdEvent &> fdMethodCommand_;
	const MethodCommand1<void, DispatcherTester, const TimerEvent &> timerMethodCommand_;
	MyDemuxer *dmx_;
	Dispatcher *disp_;
	size_t fdCommandCount_;
	size_t timerCommandCount_;

public:
	DispatcherTester()
	: fdMethodCommand_(MethodCommand1<void, DispatcherTester, const FdEvent &>(*this, &DispatcherTester::fdCommand))
	, timerMethodCommand_(MethodCommand1<void, DispatcherTester, const TimerEvent &>(*this, &DispatcherTester::timerCommand))
	{}

	void
	setUp()
	{
		dmx_ = new MyDemuxer();
		disp_ = new Dispatcher(dmx_, &DispatcherTester::now);
		fdCommandCount_ = 0;
		timerCommandCount_ = 0;
	}

	void
	tearDown()
	{
		delete disp_;
		delete dmx_;
	}

	void
	fdCommand(const FdEvent &)
	{
		++fdCommandCount_;
	}

	void
	timerCommand(const TimerEvent &)
	{
		++timerCommandCount_;
	}

	void
	testFdAction()
	{
		Mocked demux("demux");
		Fd fd(42);

		disp_->add(FdEvent(fd, FdEvent::READ), fdMethodCommand_);
		demux.expectf("%d%d", 1, 42);
		disp_->stepSingleThread();
		CPPUNIT_ASSERT_EQUAL((size_t)1, fdCommandCount_);
	}

	void
	testInvalidFdAction()
	{
		Mocked demux("demux");
		Fd fd(43);

		disp_->add(FdEvent(fd, FdEvent::READ), fdMethodCommand_);
		demux.expectf("%d%d", 1, 42);
		CPPUNIT_ASSERT_THROW(disp_->collectEvents(disp_->wait()), std::runtime_error);
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

		disp_->add(t, timerMethodCommand_);
		demux.expect(0);
		now.expect(2);
		now.expect(2);
		disp_->stepSingleThread();
		CPPUNIT_ASSERT_EQUAL((size_t)1, timerCommandCount_);
	}

	void
	testLazyTimerAction()
	{
		Mocked demux("demux");
		Mocked now("now");
		Fd fd(42);
		LazyTimer lt(DiffTime::raw(2), 0, Time::raw(0));

		disp_->add(FdEvent(fd, FdEvent::READ), fdMethodCommand_);
		disp_->add(lt, timerMethodCommand_);
		demux.expectf("%d%d", 1, 42);
		now.expect(2);
		disp_->stepSingleThread();
		CPPUNIT_ASSERT_EQUAL((size_t)1, fdCommandCount_);
		CPPUNIT_ASSERT_EQUAL((size_t)1, timerCommandCount_);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(DispatcherTester);

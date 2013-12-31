#include <src/Dispatcher.hh>

#include <tests/unit/mock/Mocked.hh>
#include <tests/unit/mock/MockRegistry.hh>

#include <cppunit/extensions/HelperMacros.h>

#include <stdexcept>

class MyDemuxer : public Demuxer {
public:
	virtual void add(const Fd &fd);
	virtual FdEvents demux(const DiffTime *interval);
};

void
MyDemuxer::add(const Fd &fd)
{
	(void)fd;
}

MyDemuxer::FdEvents
MyDemuxer::demux(const DiffTime *interval)
{
	FdEvents result;
	Mocked &m = MockRegistry::find("demux");

	(void)interval;

	for (int i = m.expectedInt(); i; --i) {
		result.push_back(FdEvent(Fd(m.expectedInt()), FdEvent::READ));
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

	const MethodCommand1<void, DispatcherTester, const FdEvent &> fdMethodCommand_;
	const MethodCommand0<void, DispatcherTester> timerMethodCommand_;
	MyDemuxer *dmx_;
	Dispatcher *disp_;
	size_t fdCommandCount_;
	size_t timerCommandCount_;

public:
	DispatcherTester()
	: fdMethodCommand_(MethodCommand1<void, DispatcherTester, const FdEvent &>(*this, &DispatcherTester::fdCommand))
	, timerMethodCommand_(MethodCommand0<void, DispatcherTester>(*this, &DispatcherTester::timerCommand))
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
		disp_->quit();
	}

	void
	timerCommand()
	{
		++timerCommandCount_;
		disp_->quit();
	}

	void
	testFdAction()
	{
		Mocked demux("demux");
		Fd fd(42);

		disp_->add(fd, fdMethodCommand_);
		demux.expectf("%d%d", 1, 42);
		disp_->step();
		CPPUNIT_ASSERT_EQUAL((size_t)1, fdCommandCount_);
	}

	void
	testInvalidFdAction()
	{
		Mocked demux("demux");
		Fd fd(43);

		disp_->add(fd, fdMethodCommand_);
		demux.expectf("%d%d", 1, 42);
		CPPUNIT_ASSERT_THROW(disp_->step(), std::runtime_error);
	}

	void
	testRun()
	{
		Mocked demux("demux");
		Fd fd(42);

		disp_->add(fd, fdMethodCommand_);
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

		disp_->add(t, timerMethodCommand_);
		demux.expect(0);
		now.expect(2);
		now.expect(2);
		CPPUNIT_ASSERT_NO_THROW(disp_->run());
		CPPUNIT_ASSERT_EQUAL((size_t)1, timerCommandCount_);
	}

	void
	testLazyTimerAction()
	{
		Mocked demux("demux");
		Mocked now("now");
		Fd fd(42);
		LazyTimer lt(DiffTime::raw(2), 0, Time::raw(0));

		disp_->add(fd, fdMethodCommand_);
		disp_->add(lt, timerMethodCommand_);
		demux.expectf("%d%d", 1, 42);
		now.expect(2);
		now.expect(2);
		disp_->step();
		CPPUNIT_ASSERT_EQUAL((size_t)1, fdCommandCount_);
		CPPUNIT_ASSERT_EQUAL((size_t)1, timerCommandCount_);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(DispatcherTester);

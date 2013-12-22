#include <src/Fd.hh>

#include <src/ErrnoException.hh>

#include <tests/unit/mock/MockedFunction.hh>
#include <tests/unit/mock/unistd.h>

#include <cppunit/extensions/HelperMacros.h>

#include <fcntl.h>

class FdTester : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(FdTester);
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST(testRead);
	CPPUNIT_TEST(testWrite);
	CPPUNIT_TEST(testGetBlocking);
	CPPUNIT_TEST(testSetBlockingThrows);
	CPPUNIT_TEST(testSetBlockingNoChange);
	CPPUNIT_TEST(testSetBlockingChange);
	CPPUNIT_TEST(testReset);
	CPPUNIT_TEST_SUITE_END();

public:
	void
	testConstruction()
	{
		MOCK_FUNCTION_DEFAULT(close);
		Fd fd(42);
		close->expect(42);
	}

	void
	testRead()
	{
		MOCK_FUNCTION_DEFAULT(close);
		MOCK_FUNCTION_DEFAULT(read);
		Fd fd(43);
		char buf[23];

		close->expect(43);
		read->expectf("%d%p%d%d", 43, (void *)buf, (int)sizeof(buf), -1);
		CPPUNIT_ASSERT_THROW(fd.read(buf, sizeof(buf)), ErrnoException);
		read->expectf("%d%p%d%d", 43, (void *)buf, (int)sizeof(buf), 45);
		CPPUNIT_ASSERT_EQUAL((size_t)45, fd.read(buf, sizeof(buf)));
	}

	void
	testWrite()
	{
		MOCK_FUNCTION_DEFAULT(close);
		MOCK_FUNCTION_DEFAULT(write);
		Fd fd(78);
		char buf[6];

		close->expect(78);
		write->expectf("%d%p%d%d", 78, (void *)buf, (int)sizeof(buf), -1);
		CPPUNIT_ASSERT_THROW(fd.write(buf, sizeof(buf)), ErrnoException);
		write->expectf("%d%p%d%d", 78, (void *)buf, (int)sizeof(buf), (int)sizeof(buf));
		CPPUNIT_ASSERT_EQUAL(sizeof(buf), fd.write(buf, sizeof(buf)));
	}

	void
	testGetBlocking()
	{
		MOCK_FUNCTION_DEFAULT(close);
		MOCK_FUNCTION_DEFAULT(fcntl);
		Fd fd(20);

		close->expect(20);
		fcntl->expectf("%d%d%d%d", 20, F_GETFL, 0, -1);
		CPPUNIT_ASSERT_THROW(fd.blocking(), ErrnoException);
		fcntl->expectf("%d%d%d%d", 20, F_GETFL, 0, 0);
		CPPUNIT_ASSERT_EQUAL(true, fd.blocking());
		fcntl->expectf("%d%d%d%d", 20, F_GETFL, 0, O_NONBLOCK);
		CPPUNIT_ASSERT_EQUAL(false, fd.blocking());
	}

	void
	testSetBlockingThrows()
	{
		MOCK_FUNCTION_DEFAULT(close);
		MOCK_FUNCTION_DEFAULT(fcntl);
		Fd fd(26);

		close->expect(26);
		fcntl->expectf("%d%d%d%d", 26, F_GETFL, 0, -1);
		CPPUNIT_ASSERT_THROW(fd.blocking(true), ErrnoException);
		fcntl->expectf("%d%d%d%d", 26, F_GETFL, 0, O_ASYNC);
		fcntl->expectf("%d%d%d%d", 26, F_SETFL, O_ASYNC | O_NONBLOCK, -1);
		CPPUNIT_ASSERT_THROW(fd.blocking(false), ErrnoException);
	}

	void
	testSetBlockingNoChange()
	{
		MOCK_FUNCTION_DEFAULT(close);
		MOCK_FUNCTION_DEFAULT(fcntl);
		Fd fd(23);

		close->expect(23);
		fcntl->expectf("%d%d%d%d", 23, F_GETFL, 0, 0);
		CPPUNIT_ASSERT_NO_THROW(fd.blocking(true));
		fcntl->expectf("%d%d%d%d", 23, F_GETFL, 0, O_NONBLOCK);
		CPPUNIT_ASSERT_NO_THROW(fd.blocking(false));
	}

	void
	testSetBlockingChange()
	{
		MOCK_FUNCTION_DEFAULT(close);
		MOCK_FUNCTION_DEFAULT(fcntl);
		Fd fd(24);

		close->expect(24);
		fcntl->expectf("%d%d%d%d", 24, F_GETFL, 0, O_ASYNC);
		fcntl->expectf("%d%d%d%d", 24, F_SETFL, O_ASYNC | O_NONBLOCK, 0);
		CPPUNIT_ASSERT_NO_THROW(fd.blocking(false));
		fcntl->expectf("%d%d%d%d", 24, F_GETFL, 0, O_ASYNC | O_NONBLOCK);
		fcntl->expectf("%d%d%d%d", 24, F_SETFL, O_ASYNC, 0);
		CPPUNIT_ASSERT_NO_THROW(fd.blocking(true));
	}

	void
	testReset()
	{
		MOCK_FUNCTION_DEFAULT(close);
		Fd fd(33);

		close->expect(33);
		fd.reset(34);
		close->expect(34);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(FdTester);

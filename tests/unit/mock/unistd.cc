#include "unistd.h"

#include <tests/unit/mock/MockRegistry.hh>
#include <tests/unit/mock/Mocked.hh>

#include </usr/include/cppunit/TestAssert.h>

REDIRECT_MOCK_C_FUNCTION1(close, void, int, fd)
REDIRECT_MOCK_C_FUNCTION3(read, ssize_t, int, fd, void *, buf, size_t, count)
REDIRECT_MOCK_C_FUNCTION3(write, ssize_t, int, fd, const void *, buf, size_t, count)

void
mock_close(int fd)
{
	Mocked &m = MockRegistry::find("close");
	CPPUNIT_ASSERT_EQUAL(m.expectedInt(), fd);
}

ssize_t
mock_read(int fd, void *buf, size_t count)
{
	Mocked &m = MockRegistry::find("read");
	CPPUNIT_ASSERT_EQUAL(m.expectedInt(), fd);
	CPPUNIT_ASSERT_EQUAL(m.expectedPointer(), (void *)buf);
	CPPUNIT_ASSERT_EQUAL(m.expectedInt(), (int)count);
	return m.expectedInt();
}

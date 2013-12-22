#ifndef TESTS_UNIT_MOCK_UNISTD_HEADER
#define TESTS_UNIT_MOCK_UNISTD_HEADER

#include <tests/unit/mock/RedirectMockCFunction.h>

#include <unistd.h>

DECLARE_MOCK_C_FUNCTION1(close, void, int)
void mock_close(int fd);

DECLARE_MOCK_C_FUNCTION3(read, ssize_t, int, void *, size_t)
ssize_t mock_read(int fd, void *buf, size_t count);

DECLARE_MOCK_C_FUNCTION3(write, ssize_t, int, const void *, size_t)
ssize_t mock_read(int fd, const void *buf, size_t count);

#endif // TESTS_UNIT_MOCK_UNISTD_HEADER

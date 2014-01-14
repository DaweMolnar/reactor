#ifndef REACTOR_UTIL_DIFFTIME_HEADER
#define REACTOR_UTIL_DIFFTIME_HEADER

#include <stdint.h>

namespace util {

class DiffTime {
	int64_t raw_;

	explicit DiffTime(int64_t raw) : raw_(raw) {}

public:
	static DiffTime raw(int64_t raw) { return DiffTime(raw); }
	static DiffTime ms(int32_t ms);

	DiffTime() : raw_(0) {}

	int64_t raw() const { return raw_; }
	int32_t ms() const;
	bool positive() const { return raw_ > 0; }
};

} // namespace util

#endif // REACTOR_UTIL_DIFFTIME_HEADER

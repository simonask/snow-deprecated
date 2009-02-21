#ifndef UTIL_H_LBLH2YH8
#define UTIL_H_LBLH2YH8

#include <ostream>
#include <stdexcept>
#include "lib/SnowAssert.h"

namespace snow {
	void warn(const char* msg, ...);
	void error(const char* msg, ...);
	#ifdef DEBUG
	void debug(const char* msg, ...);
	#else
	#define debug(...) {}
	#endif
	
	void print_mem_raw(void* start, void* end);
	
	template<typename X, typename Y>
	void print_mem(X* start, Y* end) {
		print_mem_raw((void*)start, (void*)end);
	}
	
	inline void stream_printf(std::ostream& o, const char* s) {
		while (*s) {
			if (*s == '%' && *++s != '%')
				TRAP("invalid format string: missing arguments");
			o << *s++;
		}
	}
	
	template <typename T, typename... A>
	void stream_printf(std::ostream& o, const char* s, const T& value, const A&... args) {
		while (*s) {
			if (*s == '%' && *++s != '%') {
				o << value;
				return stream_printf(o, ++s, args...);
			}
			o << *s++;
		}
		TRAP("extra arguments passed to stream_printf");
	}
}

#endif /* end of include guard: UTIL_H_LBLH2YH8 */

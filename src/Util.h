#ifndef UTIL_H_LBLH2YH8
#define UTIL_H_LBLH2YH8

namespace snot {
	void warn(const char* msg, ...);
	void error(const char* msg, ...);
	
	void print_mem_raw(void* start, void* end);
	
	template<typename X, typename Y>
	void print_mem(X* start, Y* end) {
		print_mem_raw((void*)start, (void*)end);
	}
}

#endif /* end of include guard: UTIL_H_LBLH2YH8 */

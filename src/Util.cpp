#include "Util.h"
#include "Basic.h"
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

namespace snot {
	void warn(const char* msg, ...) {
		va_list ap;
		va_start(ap, msg);
		char* interp;
		vasprintf(&interp, msg, ap);
		fprintf(stderr, "WARNING: %s\n", interp);
		free(interp);
		va_end(ap);
	}
	
	void error(const char* msg, ...) {
		va_list ap;
		va_start(ap, msg);
		char* interp;
		vasprintf(&interp, msg, ap);
		fprintf(stderr, "ERROR: %s\n", interp);
		free(interp);
		va_end(ap);
	}
	
	void print_mem_raw(void* start, void* end) {
		byte* i = (byte*)start;
		byte* j = (byte*)end;
		for (; i < j; ++i) {
			if (((int64_t)i-(int64_t)start) % 16 == 0)
				printf("%lx  ", i);
			printf("%.2x ", *i);
			if (((int64_t)i-(int64_t)start) % 16 == 15)
				printf("\n");
		}
		puts("");
	}
}

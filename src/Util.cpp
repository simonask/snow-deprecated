#include "Util.h"
#include "Basic.h"
#include <stdio.h>
#include <stdint.h>

namespace snot {
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

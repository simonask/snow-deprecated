#ifndef INCREMENTALALLOC_H_H2HJVAWU
#define INCREMENTALALLOC_H_H2HJVAWU

#include <stdlib.h>

namespace snot {
/*
	This allocation strategy is deliberately leaking!
	It is meant for objects that are supposed to live forever (more or less),
	such as compiled code.
*/

void* incremental_alloc(size_t);
void incremental_free(void* ptr);
void incremental_make_executable(void* ptr);
void incremental_make_writable(void* ptr);

class IncrementalAlloc {
public:
	void* operator new(size_t len) { return incremental_alloc(len); }
	void operator delete(void* ptr) { incremental_free(ptr); }
};
}

#endif /* end of include guard: INCREMENTALALLOC_H_H2HJVAWU */

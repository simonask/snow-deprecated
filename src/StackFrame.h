#ifndef STACKFRAME_H_2KU3CZYC
#define STACKFRAME_H_2KU3CZYC

#include "Basic.h"
#include "lib/Value.h"

namespace snow {
	struct StackFrame {
		StackFrame* previous;
		uint64_t num_locals;
		VALUE* stack_values;
	};
	
	void create_stack_frame(StackFrame*);
}

#endif /* end of include guard: STACKFRAME_H_2KU3CZYC */

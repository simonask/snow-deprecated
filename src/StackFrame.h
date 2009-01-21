#ifndef STACKFRAME_H_2KU3CZYC
#define STACKFRAME_H_2KU3CZYC

#include "Basic.h"
#include "lib/Value.h"

namespace snow {
	struct StackFrame {
		StackFrame* previous;
		VALUE self;
		VALUE call_self;
		uint64_t num_args;
		uint64_t num_named_args;
		VALUE* args;
		uint64_t num_locals;
		VALUE* locals;
	};
	
	void init_stack_frame(StackFrame*);
}

#endif /* end of include guard: STACKFRAME_H_2KU3CZYC */

#ifndef STACKFRAME_H_7CBQTUXY
#define STACKFRAME_H_7CBQTUXY

#include "Basic.h"
#include "lib/Value.h"

namespace snow {
	class Scope;
	
	// The pointers here must be handled explicitly by the GC!
	struct StackFrame {
		const char* file;
		size_t line;
		const char* funcname;
		StackFrame* previous; // calling function
		Scope* scope;
		VALUE self;
		VALUE it;
		VALUE* locals;        // == scope->m_Locals->data()
		VALUE* args;          // == scope->m_Arguments->data()
		size_t num_args;
		VALUE* temporaries;
		size_t num_temporaries;
	};
}

#endif /* end of include guard: STACKFRAME_H_7CBQTUXY */

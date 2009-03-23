#ifndef STACKFRAME_H_7CBQTUXY
#define STACKFRAME_H_7CBQTUXY

#include "lib/Value.h"
#include "lib/Scope.h"

namespace snow {
	class Scope;
	
	// The pointers here must be handled explicitly by the GC!
	struct StackFrame {
		StackFrame* previous; // calling function
		Scope* scope;
		VALUE self;
		VALUE it;
		VALUE* locals;        // == scope->m_Locals->data()
		VALUE* arguments;     // == scope->m_Arguments->data()
	};
}

#endif /* end of include guard: STACKFRAME_H_7CBQTUXY */

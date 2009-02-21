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
		// The locals pointer is identical to the corresponding member
		// of Scope. It is only here for convenience in the codegen.
		VALUE* locals;
	};
}

#endif /* end of include guard: STACKFRAME_H_7CBQTUXY */

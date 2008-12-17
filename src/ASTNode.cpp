#include "ASTNode.h"
#include <assert.h>

#define __ m.

namespace snot { namespace ast {
	void ScopeNode::emit(Codegen& m) const {
		__ function_entry(8);
		if (child())
			child()->emit(m);
		__ function_return();
	}
}}

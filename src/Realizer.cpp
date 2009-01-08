#include "Realizer.h"
#include <vector>
using namespace std;

#define __ codegen->

namespace snow {
	RefPtr<CompiledCode> Realizer::realize(const ast::Scope& scope) {
		RefPtr<Codegen> codegen = Codegen::create();
		
		vector<RefPtr<CompiledCode>> related;
		
		int num_locals = scope.sequence->count_locals();
		debug("NUM LOCALS: %d", num_locals);
		
		__ function_entry(num_locals);
		__ function_return();
		
		RefPtr<CompiledCode> code = codegen->compile();
		for each (iter, related) {
			code->add_related(*iter);
		}
		
		return code;
	}
}
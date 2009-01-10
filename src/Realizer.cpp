#include "Realizer.h"
#include <vector>
using namespace std;

#define __ codegen->

namespace snow {
	RefPtr<CompiledCode> Realizer::realize(const ast::Scope& scope) {
		RefPtr<Codegen> codegen = Codegen::create();
		
		vector<RefPtr<CompiledCode>> related;
		
		int num_locals = scope.sequence->count_locals();
		num_locals += 1 + scope.arguments.size();	// "self" + args
		
		__ function_entry(num_locals);
		// place meat here
		__ function_return();
		
		RefPtr<CompiledCode> code = codegen->compile();
		for each (iter, related) {
			code->add_related(*iter);
		}
		
		return code;
	}
}
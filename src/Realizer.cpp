#include "Realizer.h"
#include <vector>
using namespace std;

#define __ codegen->

namespace snow {
	RefPtr<CompiledCode> Realizer::realize(const ast::FunctionDefinition& def) {
		RefPtr<Codegen> codegen = Codegen::create();
		
		vector<RefPtr<CompiledCode>> related;
		
		RefPtr<Scope> scope = new Scope;
		scope->locals().add("self");
		
		for each (iter, def.arguments) {
			scope->locals().add((*iter)->name);
		}
		def.sequence->export_locals(scope->locals());
		
		int num_locals = scope->locals().size();
		debug("num_locals: %d", num_locals);
		__ function_entry(num_locals);
		int i = 0;
		for each (iter, def.arguments) {
			__ get_argument(i, scope->locals().get((*iter)->name));
			++i;
		}
		__ debug_break();
		__ function_return();
		
		RefPtr<CompiledCode> code = codegen->compile();
		for each (iter, related) {
			code->add_related(*iter);
		}
		
		return code;
	}
}
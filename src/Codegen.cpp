#include "Codegen.h"

namespace snot {
	using namespace snot::ast;
	
	void Codegen::realize(const Node& node) {
		if (node.is_a<ast::Scope>())
			realize(*node.as<ast::Scope>());
	}
	
	void Codegen::realize(const ast::Scope& node) {
		
	}
}
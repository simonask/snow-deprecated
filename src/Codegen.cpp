#include "Codegen.h"

namespace snow {
	using namespace snow::ast;
	
	void Codegen::realize(const Node& node) {
		if (node.is_a<ast::Scope>())
			realize(*node.as<ast::Scope>());
	}
	
	void Codegen::realize(const ast::Scope& node) {
		
	}
}
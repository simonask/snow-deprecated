#include "XMLCodegen.h"
#include <sstream>

namespace snow {
	std::string XMLCodegen::indent(int level) {
		level += m_BaseIndent;
		std::stringstream ss;
		while (level) {
			ss << ' ';
			level--;
		}
		return ss.str();
	}
	
	
	RefPtr<CompiledCode> XMLCodegen::compile() {
		m_Out << indent() << "<function-definition "
		return NULL;
	}
	
	void XMLCodegen::compile(ast::Literal&);
	void XMLCodegen::compile(ast::Identifier&);
	void XMLCodegen::compile(ast::Sequence&);
	void XMLCodegen::compile(ast::FunctionDefinition&);
	void XMLCodegen::compile(ast::Return&);
	void XMLCodegen::compile(ast::Throw&) {} // FIX ME
	void XMLCodegen::compile(ast::Break&) {} // FIX ME
	void XMLCodegen::compile(ast::Continue&) {} // FIX ME
	void XMLCodegen::compile(ast::Assignment&);
	void XMLCodegen::compile(ast::IfCondition&);
	void XMLCodegen::compile(ast::IfElseCondition&);
	void XMLCodegen::compile(ast::Call&);
	void XMLCodegen::compile(ast::Get&);
	void XMLCodegen::compile(ast::Set&);
	void XMLCodegen::compile(ast::Loop&);
}
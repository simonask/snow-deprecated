#ifndef XMLCODEGEN_H_5XIXBP9F
#define XMLCODEGEN_H_5XIXBP9F

#include "Codegen.h"
#include <ostream>
#include <string>

namespace snow {
	class XMLCodegen : public Codegen {
	private:
		std::ostream& m_Out;
		int m_BaseIndent;
		
		std::string indent(int level = 0);
	public:
		Codegen(ast::FunctionDefinition& def, std::ostream& os, int baseindent = 0) : Codegen(def), m_Out(os), m_BaseIndent(baseindent) {}
		
		RefPtr<CompiledCode> compile();
	private:
		void compile(ast::Literal&);
		void compile(ast::Identifier&);
		void compile(ast::Sequence&);
		void compile(ast::FunctionDefinition&);
		void compile(ast::Return&);
		void compile(ast::Throw&) {} // FIX ME
		void compile(ast::Break&) {} // FIX ME
		void compile(ast::Continue&) {} // FIX ME
		void compile(ast::Assignment&);
		void compile(ast::IfCondition&);
		void compile(ast::IfElseCondition&);
		void compile(ast::Call&);
		void compile(ast::Get&);
		void compile(ast::Set&);
		void compile(ast::Loop&);	
	};
}

#endif /* end of include guard: XMLCODEGEN_H_5XIXBP9F */

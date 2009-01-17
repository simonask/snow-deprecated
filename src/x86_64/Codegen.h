#ifndef CODEGEN_H_1VNJIVK0
#define CODEGEN_H_1VNJIVK0

#include "../Codegen.h"
#include "Scope.h"
#include "x86_64/Assembler.h"
#include <vector>
#include <list>

namespace snow {
namespace x86_64 {
	class Codegen : public snow::Codegen {
	private:
		RefPtr<x86_64::Assembler> m_Asm;
		RefPtr<Scope> m_Scope;
		std::vector<RefPtr<CompiledCode>> m_Related;
		
		std::vector<const Register*> m_PreservedTempRegisters;
		void preserve_tmp_reg(int index);
		
		void find_locals(const ast::FunctionDefinition& def, Scope&);
		void establish_stack_frame(const RefPtr<x86_64::Assembler>&, int num_locals);
		
	public:
		Codegen(ast::FunctionDefinition&);
		RefPtr<CompiledCode> compile();
		void compile(ast::Literal&);
		void compile(ast::Identifier&);
		void compile(ast::Sequence&);
		void compile(ast::FunctionDefinition&);
		void compile(ast::Assignment&);
		void compile(ast::IfCondition&);
		void compile(ast::IfElseCondition&);
		void compile(ast::Call&);
		void compile(ast::Send&);
		void compile(ast::Loop&);
	};
}
}

#endif /* end of include guard: CODEGEN_H_1VNJIVK0 */

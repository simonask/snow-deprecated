#ifndef CODEGEN_H_1VNJIVK0
#define CODEGEN_H_1VNJIVK0

#include "Codegen.h"
#include "x86_64/x86_64-Assembler.h"
#include <vector>
#include <list>

namespace snow {
namespace x86_64 {
	class Codegen : public snow::Codegen {
	private:
		bool m_InGlobalScope;
		LocalMap* m_LocalMap;
		RefPtr<x86_64::Assembler> m_Asm;
		RefPtr<Label> m_Return;
		std::vector<CompiledCode*> m_Related;
		uint64_t m_NumLocals;
		uint64_t m_NumStackArguments;
		uint64_t m_NumTemporaries;
		std::vector<uint64_t> m_FreeTemporaries;
		std::string m_AssignmentName;
		bool m_AssignmentFunction;

		uint64_t reserve_temporary();
		void free_temporary(uint64_t id);
		
		void get_local(uint64_t id, const Register& reg);
		void set_local(const Register& reg, uint64_t id, const Register& tmp = rbx);
	public:
		Codegen(ast::FunctionDefinition&);
		CompiledCode* compile(bool in_global_scope = false);
		static CompiledCode* compile_proxy(void* function_pointer, const ExternalLibrary::FunctionSignature&); 
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
		void compile(ast::IfElseIfElseCondition&);
		void compile(ast::Call&);
		void compile(ast::Get&);
		void compile(ast::Set&);
		void compile(ast::Loop&);
		void compile(ast::Self&);
		void compile(ast::It&);
	};
}
}

#endif /* end of include guard: CODEGEN_H_1VNJIVK0 */

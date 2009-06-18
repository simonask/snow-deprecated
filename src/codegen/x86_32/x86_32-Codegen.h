#ifndef CODEGEN_H_1VNJIVK0
#define CODEGEN_H_1VNJIVK0

#include "Codegen.h"
#include "x86_32-Assembler.h"
#include <vector>
#include <list>

namespace snow {
namespace x86_32 {
	class Codegen : public snow::Codegen {
	private:
		bool m_InGlobalScope;
		LocalMap* m_LocalMap;
		RefPtr<x86_32::Assembler> m_Asm;
		RefPtr<Label> m_Return;
		std::vector<CompiledCode*> m_Related;
		uintx m_NumLocals;
		uintx m_NumStackArguments;
		uintx m_NumTemporaries;
		std::vector<uintx> m_FreeTemporaries;
		std::string m_AssignmentName;
		bool m_AssignmentFunction;

		uintx reserve_temporary();
		void free_temporary(uintx id);
		
		void get_local(uintx id, const Register& reg);
		void set_local(const Register& reg, uintx id, const Register& tmp = rbx);
		void generate_store_arguments_for_call(uintx* tmps, const RefPtr<ast::Sequence>& args);
		void generate_refetch_arguments_for_call(uintx* tmps, size_t num_args);
	public:
		Codegen(ast::FunctionDefinition&);
		CompiledCode* compile(bool in_global_scope = false);
		static CompiledCode* compile_proxy(void* function_pointer, const ExternalLibrary::FunctionSignature&); 
	private:
		void compile(ast::Literal&);
		void compile(ast::Identifier&);
		void compile(ast::Self&);
		void compile(ast::It&);
		void compile(ast::Sequence&);
		void compile(ast::FunctionDefinition&);
		void compile(ast::Return&);
		void compile(ast::Break&) {} // TODO
		void compile(ast::Continue&) {} // TODO
		void compile(ast::LocalAssignment&);
		void compile(ast::MemberAssignment&);
		void compile(ast::Member&);
		void compile(ast::IfCondition&);
		void compile(ast::IfElseCondition&);
		void compile(ast::IfElseIfElseCondition&);
		void compile(ast::ExpressionCall&);
		void compile(ast::MemberCall&);
		void compile(ast::Loop&);
		void compile(ast::LogicalAnd&);
		void compile(ast::LogicalOr&);
		void compile(ast::LogicalXor&);
	};
}
}

#endif /* end of include guard: CODEGEN_H_1VNJIVK0 */

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
		template <typename T> friend void set_argument(Codegen&, uint64_t, T);
		template <typename T> friend void get_argument(Codegen&, uint64_t, T);
	private:
		RefPtr<x86_64::Assembler> m_Asm;
		RefPtr<Scope> m_Scope;
		RefPtr<Label> m_Return;
		std::vector<RefPtr<CompiledCode>> m_Related;
		uint64_t m_NextTemporary;
		uint64_t m_NumStackArguments;
		
		void find_locals();
		int num_locals() const;
		void establish_stack_frame(const RefPtr<x86_64::Assembler>&, int num_locals);
		
		int offset_for_stack_frame() { return -(int)sizeof(StackFrame); }
		int offset_for_locals();
		int offset_for_local(const Scope::Local& local);
		Address address_for_local(const Scope::Local& local);
		int offset_for_temporaries();
		int offset_for_temporary(int id);
		Address address_for_temporary(int id);
		Address create_temporary();
	public:
		Codegen(ast::FunctionDefinition&);
		RefPtr<CompiledCode> compile();
		void compile(ast::Literal&);
		void compile(ast::Identifier&);
		void compile(ast::Sequence&);
		void compile(ast::FunctionDefinition&);
		void compile(ast::Return&);
		void compile(ast::Assignment&);
		void compile(ast::IfCondition&);
		void compile(ast::IfElseCondition&);
		void compile(ast::Call&);
		void compile(ast::MethodCall&);
		void compile(ast::Send&);
		void compile(ast::Loop&);
	};
}
}

#endif /* end of include guard: CODEGEN_H_1VNJIVK0 */

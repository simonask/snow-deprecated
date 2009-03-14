#ifndef CODEGEN_H_1VNJIVK0
#define CODEGEN_H_1VNJIVK0

#include "../Codegen.h"
#include "x86_64/Assembler.h"
#include <vector>
#include <list>

namespace snow {
namespace x86_64 {
	class Codegen : public snow::Codegen {
	private:
		Handle<LocalMap> m_LocalMap;
		RefPtr<x86_64::Assembler> m_Asm;
		RefPtr<Label> m_Return;
		std::vector<RefPtr<CompiledCode>> m_Related;
		uint64_t m_NumLocals;
		uint64_t m_NumTemporaries;
		uint64_t m_NumStackArguments;
		uint64_t reserve_temporary() { return m_NumTemporaries++; }
		
		void get_local(uint64_t id, const Register& reg);
		void set_local(const Register& reg, uint64_t id, const Register& tmp = rbx);
		void get_temporary(uint64_t id, const Register& reg);
		void set_temporary(const Register& reg, uint64_t id);
	public:
		Codegen(ast::FunctionDefinition&);
		RefPtr<CompiledCode> compile();
	private:
		void compile(ast::Literal&);
		void compile(ast::Identifier&);
		void compile(ast::Sequence&);
		void compile(ast::FunctionDefinition&);
		void compile(ast::Return&);
		void compile(ast::Assignment&);
		void compile(ast::IfCondition&);
		void compile(ast::IfElseCondition&);
		void compile(ast::Call&);
		void compile(ast::Get&);
		void compile(ast::Set&);
		void compile(ast::Loop&);
	};
}
}

#endif /* end of include guard: CODEGEN_H_1VNJIVK0 */

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
		std::vector<const Register*> m_PreservedTempRegisters;
		void preserve_tmp_reg(int index);
		
		void find_locals(const ast::FunctionDefinition& def, Scope::LocalList&);
		void establish_stack_frame(const RefPtr<x86_64::Assembler>&, int num_locals);
	public:
		RefPtr<CompiledCode> compile(const ast::FunctionDefinition& def);
	};
}
}

#endif /* end of include guard: CODEGEN_H_1VNJIVK0 */

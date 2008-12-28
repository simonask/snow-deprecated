#ifndef CODEGEN_H_1VNJIVK0
#define CODEGEN_H_1VNJIVK0

#include "../Codegen.h"
#include "Scope.h"
#include "x86_64/Assembler.h"
#include <vector>
#include <list>

namespace snot {
namespace x86_64 {
	class Codegen : public snot::Codegen {
	private:
		struct ScopeData {
			x86_64::Assembler* enter_asm;
			x86_64::Assembler* leave_asm;
			ScopeData() : enter_asm(NULL), leave_asm(NULL) {}
			ScopeData(x86_64::Assembler* e, x86_64::Assembler* l) : enter_asm(e), leave_asm(l) {}
		};
		
		x86_64::Assembler m_Assembler;
		
		std::list<ScopeData> m_ScopeData;
		ScopeData m_CurrentScope;
		
		std::vector<const Register*> m_PreservedTempRegisters;
		void preserve_tmp_reg(int index);
	public:
		CompiledCode compile();
		Scope function_entry(int num_locals);
		void function_return();
		void set_argument(int index, const Scope::Local&);
		void set_argument(int index, const Scope::Temporary&);
		void set_argument(int index, const void* ptr);
		void set_argument(int index, int immediate);
		void get_argument(int index, const Scope::Local&);
		void get_argument(int index, const Scope::Temporary&);
		void set_return(const Scope::Local& src);
		void set_return(const Scope::Temporary& src);
		void call(const char* symbol, const Scope::Local& retval);
		void call(const char* symbol, const Scope::Temporary& retval);
		void debug_break() { m_Assembler.debug_break(); }
	};
}
}

#endif /* end of include guard: CODEGEN_H_1VNJIVK0 */

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
		struct ScopeData {
			uint64_t num_locals;
			RefPtr<x86_64::Assembler> enter_asm;
			RefPtr<x86_64::Assembler> leave_asm;
			ScopeData(uint64_t nl, RefPtr<x86_64::Assembler> e, RefPtr<x86_64::Assembler> l) : num_locals(nl), enter_asm(e), leave_asm(l) {}
		};
		
		x86_64::Assembler m_Assembler;
		
		std::list<RefPtr<ScopeData>> m_ScopeDataList;
		RefPtr<ScopeData> m_CurrentScope;
		
		std::vector<const Register*> m_PreservedTempRegisters;
		void preserve_tmp_reg(int index);
	public:
		RefPtr<CompiledCode> compile();
		Scope function_entry(int num_locals);
		void function_return();
		void set_local(const Local& dst, const Local& src);
		void set_local(const Local& dst, VALUE constant);
		void set_argument(int index, const Local&);
		void set_argument(int index, const Scope::Temporary&);
		void set_argument(int index, const void* ptr);
		void set_argument(int index, int immediate);
		void get_argument(int index, const Local&);
		void get_argument(int index, const Scope::Temporary&);
		void set_return(const Local& src);
		void set_return(const Scope::Temporary& src);
		void call(const char* symbol);
		void call(const char* symbol, const Local& retval);
		void call(const char* symbol, const Scope::Temporary& retval);
		void debug_break() { m_Assembler.debug_break(); }
	};
}
}

#endif /* end of include guard: CODEGEN_H_1VNJIVK0 */

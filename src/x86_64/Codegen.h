#ifndef CODEGEN_H_1VNJIVK0
#define CODEGEN_H_1VNJIVK0

#include "../Codegen.h"
#include "x86_64/Assembler.h"
#include <vector>

namespace snot {
namespace x86_64 {
	class Codegen : public snot::Codegen {
	private:
		x86_64::Assembler m_Assembler;
		
		Address addr_for_local(const Local&);
		const Register& reg_for_arg(int index);
	public:
		CompiledCode compile();
		Local local(const char* name = 0);
		void function_entry(int num_locals);
		void function_return();
		void set_argument(int index, const Local&);
		void set_argument(int index, const void* ptr);
		void set_argument(int index, int immediate);
		void get_argument(int index, const Local&);
		void set_return(const Local& src);
		void call(const char* symbol, const Local& retval);
	};
}
}

#endif /* end of include guard: CODEGEN_H_1VNJIVK0 */

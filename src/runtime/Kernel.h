#ifndef KERNEL_H_YA3CD846
#define KERNEL_H_YA3CD846

#include "base/Basic.h"
#include "runtime/Scope.h"
#include "codegen/Linker.h"

namespace snow {
	class Kernel {
	private:
		Kernel() {}
	public:
		static void init();
		static Linker::SymbolTable& linker_symbols();
		static Scope& global_scope();
		
		static VALUE require(const std::string& file);
		static VALUE eval(const std::string& str);
		static VALUE eval_in_global_scope(const std::string& str);
	};
}

#endif /* end of include guard: KERNEL_H_YA3CD846 */

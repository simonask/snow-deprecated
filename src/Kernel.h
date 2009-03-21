#ifndef KERNEL_H_YA3CD846
#define KERNEL_H_YA3CD846

#include "Basic.h"
#include "lib/Scope.h"
#include "SymbolTable.h"

namespace snow {
	class Kernel {
	private:
		Kernel() {}
	public:
		static void init();
		static SymbolTable& runtime_symbols();
		static Scope& global_scope();
		
		static VALUE require(const std::string& file);
	};
}

#endif /* end of include guard: KERNEL_H_YA3CD846 */

#include "Codegen.h"

#ifdef ARCH_x86_64
#include "x86_64/x86_64-Codegen.h"
#endif

#ifdef ARCH_x86_32
#include "x86_32/x86_32-Codegen.h"
#endif

namespace snow {
	RefPtr<Codegen> Codegen::create(ast::FunctionDefinition& def) {
		#ifdef ARCH_x86_64
		return new x86_64::Codegen(def);
		#else
		#ifdef ARCH_x86_32
		return new x86_32::Codegen(def);
		#else
		#error Current architecture is unsupported!
		#endif
		#endif
	}

	Ptr<CompiledCode> Codegen::compile_proxy(void* function_ptr, const ExternalLibrary::FunctionSignature& signature) {
		#ifdef ARCH_x86_64
		return x86_64::Codegen::compile_proxy(function_ptr, signature);
		#else
		#ifdef ARCH_x86_32
		return x86_32::Codegen::compile_proxy(function_ptr, signature);
		#else
		#error Current architecture is unsupported!
		#endif
		#endif
	}
}

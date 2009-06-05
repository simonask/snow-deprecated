#include "Codegen.h"

#ifdef __x86_64__
#include "x86_64/x86_64-Codegen.h"
#endif

namespace snow {
	RefPtr<Codegen> Codegen::create(ast::FunctionDefinition& def) {
		#ifdef __x86_64__
		return new x86_64::Codegen(def);
		#else
		#error Current architecture is unsupported!
		#endif
	}

	CompiledCode* Codegen::compile_proxy(void* function_ptr, const ExternalLibrary::FunctionSignature& signature) {
		#ifdef __x86_64__
		return x86_64::Codegen::compile_proxy(function_ptr, signature);
		#else
		#error Current architecture is unsupported!
		#endif
	}
}

#include "Codegen.h"

#ifdef __x86_64__
#include "x86_64/Codegen.h"
#endif

namespace snow {
	RefPtr<Codegen> Codegen::create() {
		#ifdef __x86_64__
		return new x86_64::Codegen();
		#else
		#error Current architecture is unsupported!
		#endif
	}
}
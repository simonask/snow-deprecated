#ifndef REALIZER_H_RGGVG5JE
#define REALIZER_H_RGGVG5JE

#include "ASTNode.h"
#include "CompiledCode.h"

namespace snow {
	class Realizer {
	public:
		Realizer() {}
		
		RefPtr<CompiledCode> realize(const ast::Scope&);
	};
}

#endif /* end of include guard: REALIZER_H_RGGVG5JE */

#ifndef CODEGEN_H_J0VKJE70
#define CODEGEN_H_J0VKJE70

#include "SymbolTable.h"
#include "CompiledCode.h"
#include "Label.h"
#include "Scope.h"
#include "ASTNode.h"
#include "StackFrame.h"

#include <string>

namespace snow {
	
class Codegen {
public:
	virtual ~Codegen() {}
	
	// Create a Codegen for the current arch
	static RefPtr<Codegen> create();
	
	virtual RefPtr<CompiledCode> compile(const ast::FunctionDefinition&) = 0;
};
}

#endif /* end of include guard: CODEGEN_H_J0VKJE70 */

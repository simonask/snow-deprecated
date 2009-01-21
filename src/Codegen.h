#ifndef CODEGEN_H_J0VKJE70
#define CODEGEN_H_J0VKJE70

#include "SymbolTable.h"
#include "CompiledCode.h"
#include "Label.h"
#include "Scope.h"
#include "StackFrame.h"

#include <string>

namespace snow {
	namespace ast {
		class Literal;
		class Identifier;
		class Sequence;
		class FunctionDefinition;
		class Assignment;
		class IfCondition;
		class IfElseCondition;
		class Call;
		class MethodCall;
		class Send;
		class Loop;
	}
	
class Codegen {
protected:
	ast::FunctionDefinition& m_Def;
	Codegen(ast::FunctionDefinition& def) : m_Def(def) {}
public:
	virtual ~Codegen() {}
	
	// Create a Codegen for the current arch
	static RefPtr<Codegen> create(ast::FunctionDefinition&);
	virtual RefPtr<CompiledCode> compile() = 0;
	virtual void compile(ast::Literal&) = 0;
	virtual void compile(ast::Identifier&) = 0;
	virtual void compile(ast::Sequence&) = 0;
	virtual void compile(ast::FunctionDefinition&) = 0;
	virtual void compile(ast::Assignment&) = 0;
	virtual void compile(ast::IfCondition&) = 0;
	virtual void compile(ast::IfElseCondition&) = 0;
	virtual void compile(ast::Call&) = 0;
	virtual void compile(ast::MethodCall&) = 0;
	virtual void compile(ast::Send&) = 0;
	virtual void compile(ast::Loop&) = 0;
};
}

#endif /* end of include guard: CODEGEN_H_J0VKJE70 */

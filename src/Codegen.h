#ifndef CODEGEN_H_J0VKJE70
#define CODEGEN_H_J0VKJE70

#include "CompiledCode.h"
#include "Label.h"
#include "Handle.h"

#include <string>

namespace snow {
	namespace ast {
		class Literal;
		class Identifier;
		class Sequence;
		class FunctionDefinition;
		class Return;
        class Throw;
        class Break;
        class Continue;
		class Assignment;
		class IfCondition;
		class IfElseCondition;
		class Call;
		class Get;
		class Set;
		class Loop;
		class Self;
		class It;
	}
	
class Codegen {
protected:
	ast::FunctionDefinition& m_Def;
	Codegen(ast::FunctionDefinition& def) : m_Def(def) {}
public:
	virtual ~Codegen() {}
	
	// Create a Codegen for the current arch
	static RefPtr<Codegen> create(ast::FunctionDefinition&);
	virtual Handle<CompiledCode> compile(bool in_global_scope = false) = 0;
	virtual void compile(ast::Literal&) = 0;
	virtual void compile(ast::Identifier&) = 0;
	virtual void compile(ast::Sequence&) = 0;
	virtual void compile(ast::FunctionDefinition&) = 0;
	virtual void compile(ast::Return&) = 0;
	virtual void compile(ast::Throw&) = 0;
	virtual void compile(ast::Break&) = 0;
	virtual void compile(ast::Continue&) = 0;
	virtual void compile(ast::Assignment&) = 0;
	virtual void compile(ast::IfCondition&) = 0;
	virtual void compile(ast::IfElseCondition&) = 0;
	virtual void compile(ast::Call&) = 0;
	virtual void compile(ast::Get&) = 0;
	virtual void compile(ast::Set&) = 0;
	virtual void compile(ast::Loop&) = 0;
	virtual void compile(ast::Self&) = 0;
	virtual void compile(ast::It&) = 0;
};
}

#endif /* end of include guard: CODEGEN_H_J0VKJE70 */

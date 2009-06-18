#ifndef CODEGEN_H_J0VKJE70
#define CODEGEN_H_J0VKJE70

#include "CompiledCode.h"
#include "Label.h"
#include "runtime/Handle.h"
#include "runtime/ExternalLibrary.h"

#include <string>

namespace snow {
	namespace ast {
		struct Literal;
		struct Identifier;
		struct Self;
		struct It;
		struct Sequence;
		struct FunctionDefinition;
		struct Return;
		struct Break;
		struct Continue;
		struct LocalAssignment;
		struct MemberAssignment;
		struct Member;
		struct IfCondition;
		struct IfElseCondition;
		struct IfElseIfElseCondition;
		struct ExpressionCall;
		struct MemberCall;
		struct Loop;
		struct LogicalAnd;
		struct LogicalOr;
		struct LogicalXor;
    struct LogicalNot;
	}
class Codegen {
protected:
	ast::FunctionDefinition& m_Def;
	Codegen(ast::FunctionDefinition& def) : m_Def(def) {}
public:
	virtual ~Codegen() {}
	
	// Create a Codegen for the current arch
	static RefPtr<Codegen> create(ast::FunctionDefinition&);
	virtual CompiledCode* compile(bool in_global_scope = false) = 0;
	virtual void compile(ast::Literal&) = 0;
	virtual void compile(ast::Identifier&) = 0;
	virtual void compile(ast::Self&) = 0;
	virtual void compile(ast::It&) = 0;
	virtual void compile(ast::Sequence&) = 0;
	virtual void compile(ast::FunctionDefinition&) = 0;
	virtual void compile(ast::Return&) = 0;
	virtual void compile(ast::Break&) = 0;
	virtual void compile(ast::Continue&) = 0;
	virtual void compile(ast::LocalAssignment&) = 0;
	virtual void compile(ast::MemberAssignment&) = 0;
	virtual void compile(ast::Member&) = 0;
	virtual void compile(ast::IfCondition&) = 0;
	virtual void compile(ast::IfElseCondition&) = 0;
	virtual void compile(ast::IfElseIfElseCondition&) = 0;
	virtual void compile(ast::ExpressionCall&) = 0;
	virtual void compile(ast::MemberCall&) = 0;
	virtual void compile(ast::Loop&) = 0;
	virtual void compile(ast::LogicalAnd&) = 0;
	virtual void compile(ast::LogicalOr&) = 0;
	virtual void compile(ast::LogicalXor&) = 0;
	virtual void compile(ast::LogicalNot&) = 0;

	static CompiledCode* compile_proxy(void* function_pointer, const ExternalLibrary::FunctionSignature&);
};
}

#endif /* end of include guard: CODEGEN_H_J0VKJE70 */

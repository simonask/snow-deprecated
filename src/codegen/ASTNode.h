#ifndef NODE_H_WML097FE
#define NODE_H_WML097FE

#include "base/Basic.h"
#include "codegen/Codegen.h"
#include "runtime/Value.h"
#include "runtime/Symbol.h"
#include <string>
#include <list>
#include "gc/TempAllocator.h"

#define AST_ABSTRACT_NODE virtual void compile(Codegen&) = 0;
#define AST_NODE virtual void compile(Codegen& codegen) { codegen.compile(*this); }

namespace snow {
namespace ast {
	struct Node : public TempAllocator<Node> {
		AST_ABSTRACT_NODE;
		
		virtual ~Node() {}
		
		template <class T>
		T* as() { return dynamic_cast<T*>(this); }
		template <class T>
		const T* as() const { return dynamic_cast<const T*>(this); }
		template <class T>
		bool is_a() const { return as<T>() != NULL; }
	};
	
	struct Literal : Node {
		AST_NODE;
		
		enum Type {
			STRING_TYPE,
			INTEGER_TYPE,
			INTEGER_DEC_TYPE = INTEGER_TYPE,
			INTEGER_HEX_TYPE,
			INTEGER_BIN_TYPE,
			FLOAT_TYPE,
			TRUE_TYPE,
			FALSE_TYPE,
			NIL_TYPE,
			SYMBOL_TYPE
		};
		std::string string;
		Type type;
		
		Literal(const std::string& str, Type type) : string(str), type(type) {}
		Literal(Type type) : type(type) {}
	};
	
	struct Identifier : Node {
		AST_NODE;
		
		VALUE name;
		bool quiet;
		
		Identifier(const char* name) : name(symbol(name)), quiet(false) {}
	};
	
	struct Self : Node {
		AST_NODE;
	};
	
	struct It : Node {
		AST_NODE;
	};
	
	struct Sequence : Node {
		AST_NODE;
		
		std::list<RefPtr<Node>> nodes;
		
		Sequence() {}
		template <typename... T>
		Sequence(const RefPtr<Node>& first, const T&... args) {
			add(first, args...);
		}
		void add() { }
		template <typename... T>
		void add(const RefPtr<Node>& node, const T&... args) {
			nodes.push_back(node);
			add(args...);
		}
		
		size_t length() const { return nodes.size(); }
	};
	
	struct FunctionDefinition : Node {
		AST_NODE;
		
		const char* name;
		const char* file;
		int line;
		std::list<RefPtr<Identifier>> arguments;
		RefPtr<Sequence> sequence;
		
		FunctionDefinition() : name("<unknown function>"), file("<unknown file>"), line(0), sequence(new Sequence) {}
		template <typename... T>
		FunctionDefinition(const RefPtr<Node>& first, const T&... args) : name("<unknown function>"), file("<unknown file>"), line(0), sequence(new Sequence) { add(first, args...); }
		void add() {}
		template <typename... T>
		void add(const RefPtr<Node>& node, const T&... args) { sequence->add(node, args...); }
		void add_argument(RefPtr<Identifier> identifier) { arguments.push_back(identifier); }
	};
	
	struct Return : Node {
		AST_NODE;
		
		RefPtr<Node> expression;
		
		Return() {}
		Return(const RefPtr<Node>& expr) : expression(expr) {}
	};
	
	struct Break : Node {
		AST_NODE;
	};
	
	struct Continue : Node {
		AST_NODE;
	};
	
	struct Assignment : Node {
		AST_ABSTRACT_NODE;
		RefPtr<Node> expression;
		Assignment(RefPtr<Node> expr) : expression(expr) {}
	};
	
	struct LocalAssignment : Assignment {
		AST_NODE;
		RefPtr<Identifier> local;
		LocalAssignment(RefPtr<Identifier> local, RefPtr<Node> expr) : Assignment(expr), local(local) {}
	};
	
	struct MemberAssignment : Assignment {
		AST_NODE;
		RefPtr<Node> object;
		RefPtr<Identifier> member;
		MemberAssignment(RefPtr<Node> object, RefPtr<Identifier> member, RefPtr<Node> expr) : Assignment(expr), object(object), member(member) {}
	};
	
	struct Member : Node {
		AST_NODE;
		RefPtr<Node> object;
		RefPtr<Identifier> member;
		Member(RefPtr<Node> object, RefPtr<Identifier> member) : object(object), member(member) {}
	};
	
	struct Condition : Node {
		AST_ABSTRACT_NODE;
		RefPtr<Node> expression;
		Condition(RefPtr<Node> expr) : expression(expr) {}
	};
	
	struct IfCondition : public Condition {
		AST_NODE;
		RefPtr<Node> if_true;
		bool unless;
		IfCondition(RefPtr<Node> expr, RefPtr<Node> if_true, bool unless_expr = false) : Condition(expr), if_true(if_true), unless(unless_expr) {}
	};
	
	struct IfElseCondition : public IfCondition {
		AST_NODE;
		RefPtr<Node> if_false;
		IfElseCondition(RefPtr<Node> expr, RefPtr<Node> if_true, RefPtr<Node> if_false, bool unless_expr = false) : IfCondition(expr, if_true, unless_expr), if_false(if_false) {}
	};
	
	struct IfElseIfElseCondition : public IfElseCondition {
		AST_NODE;
		std::list<RefPtr<IfCondition>> else_if;
		IfElseIfElseCondition(RefPtr<Node> expr, RefPtr<Node> if_true, RefPtr<Node> if_false, bool unless_expr = false) : IfElseCondition(expr, if_true, if_false, unless_expr) {}
	};
	
	struct Call : Node {
		AST_ABSTRACT_NODE;
		RefPtr<Sequence> arguments;
		Call(RefPtr<Sequence> args = NULL) : arguments(args) {}
	};
	
	struct ExpressionCall : Call {
		AST_NODE;
		RefPtr<Node> expression;
		ExpressionCall(RefPtr<Node> expr, RefPtr<Sequence> args = NULL) : Call(args), expression(expr) {}
	};
	
	struct MemberCall : Call {
		AST_NODE;
		RefPtr<Node> self;
		RefPtr<Identifier> member;
		MemberCall(RefPtr<Node> self, RefPtr<Identifier> member, RefPtr<Sequence> args = NULL) : Call(args), self(self), member(member) {}
	};
	
	struct Loop : Node {
		AST_NODE;
		RefPtr<Node> expression;
		RefPtr<Node> while_true;
		Loop(RefPtr<Node> expression, RefPtr<Node> while_true) : expression(expression), while_true(while_true) {}
	};
}
}

#endif /* end of include guard: NODE_H_WML097FE */

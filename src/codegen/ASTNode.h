#ifndef NODE_H_WML097FE
#define NODE_H_WML097FE

#include "base/Basic.h"
#include "codegen/Codegen.h"
#include "runtime/Value.h"
#include "runtime/Symbol.h"
#include <string>
#include <list>
#include "gc/TempAllocator.h"

namespace snow {
namespace ast {
	struct Node : public TempAllocator<Node> {
		virtual ~Node() {}
		
		template <class T>
		T* as() { return dynamic_cast<T*>(this); }
		template <class T>
		const T* as() const { return dynamic_cast<const T*>(this); }
		template <class T>
		bool is_a() const { return as<T>() != NULL; }
		
		virtual void compile(Codegen& codegen) { TRAP("ast::Node::compile called -- maybe you forgot to override in your Node class?"); }
	};
	
	struct Literal : Node {
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
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Identifier : Node {
		VALUE name;
		bool quiet;
		
		Identifier(const char* name) : name(symbol(name)), quiet(false) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Self : Node {
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct It : Node {
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Sequence : Node {
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
		
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct FunctionDefinition : Node {
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
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Return : Node {
		RefPtr<Node> expression;
		
		Return() {}
		Return(const RefPtr<Node>& expr) : expression(expr) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Throw : Node {
		RefPtr<Node> object;
		
    	Throw(RefPtr<Node> obj) : object(obj) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }        
	};
	
	struct Break : Node {
		Break() {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }        
	};
	
	struct Continue : Node {
		Continue() {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Assignment : Node {
		RefPtr<Identifier> identifier;
		RefPtr<Node> expression;
		Assignment(RefPtr<Identifier> ident, RefPtr<Node> expr) : identifier(ident), expression(expr) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Condition : Node {
		RefPtr<Node> expression;
		Condition(RefPtr<Node> expr) : expression(expr) {}
	};
	
	struct IfCondition : public Condition {
		RefPtr<Node> if_true;
		bool unless;
		IfCondition(RefPtr<Node> expr, RefPtr<Node> if_true, bool unless_expr = false) : Condition(expr), if_true(if_true), unless(unless_expr) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct IfElseCondition : public IfCondition {
		RefPtr<Node> if_false;
		IfElseCondition(RefPtr<Node> expr, RefPtr<Node> if_true, RefPtr<Node> if_false, bool unless_expr = false) : IfCondition(expr, if_true, unless_expr), if_false(if_false) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct IfElseIfElseCondition : public IfElseCondition {
		std::list<RefPtr<IfCondition>> else_if;
		IfElseIfElseCondition(RefPtr<Node> expr, RefPtr<Node> if_true, RefPtr<Node> if_false, bool unless_expr = false) : IfElseCondition(expr, if_true, if_false, unless_expr) {}		
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Call : Node {
		RefPtr<Node> self;
		
		// if member is non-NULL, self.member is called, self is called as a function
		RefPtr<Identifier> member;
		
		RefPtr<Sequence> arguments;
		
		Call(RefPtr<Node> self, RefPtr<Identifier> member, RefPtr<Sequence> args = new Sequence) : self(self), member(member), arguments(args) {}
		Call(RefPtr<Node> self, RefPtr<Sequence> args = new Sequence) : self(self), arguments(args) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Get : Node {
		RefPtr<Node> self;
		RefPtr<Identifier> member;
		Get(RefPtr<Node> self, RefPtr<Identifier> member) : self(self), member(member) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Set : Node {
		RefPtr<Node> self;
		RefPtr<Identifier> member;
		RefPtr<Node> expression;
		Set(RefPtr<Node> self, RefPtr<Identifier> member, RefPtr<Node> expr) : self(self), member(member), expression(expr) {}
		Set(RefPtr<Get> get, RefPtr<Node> expr) : self(get->self), member(get->member), expression(expr) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Loop : Node {
		RefPtr<Node> expression;
		RefPtr<Node> while_true;
		Loop(RefPtr<Node> expression, RefPtr<Node> while_true) : expression(expression), while_true(while_true) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
}
}

#endif /* end of include guard: NODE_H_WML097FE */

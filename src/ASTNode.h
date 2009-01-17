#ifndef NODE_H_WML097FE
#define NODE_H_WML097FE

#include "Basic.h"
#include "Codegen.h"
#include <string>
#include <list>

namespace snow {
namespace ast {
	struct Node {
		virtual ~Node() {}
		
		template <class T>
		T* as() { return dynamic_cast<T*>(this); }
		template <class T>
		const T* as() const { return dynamic_cast<const T*>(this); }
		template <class T>
		bool is_a() const { return as<T>() != NULL; }
		
		virtual void export_locals(Scope&) const {}
		virtual void compile(Codegen& codegen) { throw std::runtime_error("ast::Node::compile called -- maybe you forgot to override in your Node class?"); }
	};
	
	struct Literal : Node {
		enum Type {
			STRING_TYPE,
			INTEGER_TYPE,
			INTEGER_DEC_TYPE = INTEGER_TYPE,
			INTEGER_HEX_TYPE,
			INTEGER_BIN_TYPE,
			FLOAT_TYPE,
		};
		std::string string;
		Type type;
		
		Literal(const std::string& str, Type type) : string(str), type(type) {}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Identifier : Node {
		std::string name;
		
		Identifier(const std::string& name) : name(name) {}
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
		
		virtual void export_locals(Scope& scope) const {
			for each (iter, nodes) { (*iter)->export_locals(scope); }
		}
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct FunctionDefinition : Node {
		std::list<RefPtr<Identifier>> arguments;
		RefPtr<Sequence> sequence;
		
		FunctionDefinition() : sequence(new Sequence) {}
		template <typename... T>
		FunctionDefinition(const RefPtr<Node>& first, const T&... args) : sequence(new Sequence) { add(first, args...); }
		void add() {}
		template <typename... T>
		void add(const RefPtr<Node>& node, const T&... args) { sequence->add(node, args...); }
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Assignment : Node {
		RefPtr<Identifier> identifier;
		RefPtr<Node> expression;
		Assignment(RefPtr<Identifier> ident, RefPtr<Node> expr) : identifier(ident), expression(expr) {}
		virtual void export_locals(Scope& scope) const { scope.add_local(identifier->name); expression->export_locals(scope); }
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Condition : Node {
		RefPtr<Node> expression;
		Condition(RefPtr<Node> expr) : expression(expr) {}
		virtual void export_locals(Scope& scope) const { expression->export_locals(scope); }
	};
	
	struct IfCondition : public Condition {
		RefPtr<Node> if_true;
		IfCondition(RefPtr<Node> expr, RefPtr<Node> if_true) : Condition(expr), if_true(if_true) {}
		virtual void export_locals(Scope& scope) const { Condition::export_locals(scope); if_true->export_locals(scope); }
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct IfElseCondition : public IfCondition {
		RefPtr<Node> if_false;
		IfElseCondition(RefPtr<Node> expr, RefPtr<Node> if_true, RefPtr<Node> if_false) : IfCondition(expr, if_true), if_false(if_false) {}
		virtual void export_locals(Scope& scope) const { IfCondition::export_locals(scope); if_false->export_locals(scope); }
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Call : Node {
		RefPtr<Node> object;
		RefPtr<Sequence> arguments;
		Call(RefPtr<Node> obj, RefPtr<Sequence> args = NULL) : object(obj), arguments(args) {}
		virtual void export_locals(Scope& scope) const { object->export_locals(scope); if (arguments) arguments->export_locals(scope); }
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Send : Node {
		RefPtr<Node> self;
		RefPtr<Node> message;
		Send(RefPtr<Node> self, RefPtr<Node> message) : self(self), message(message) {}
		virtual void export_locals(Scope& scope) const { self->export_locals(scope); message->export_locals(scope); }
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
	
	struct Loop : Node {
		RefPtr<Node> expression;
		RefPtr<Node> while_true;
		Loop(RefPtr<Node> expression, RefPtr<Node> while_true) : expression(expression), while_true(while_true) {}
		virtual void export_locals(Scope& scope) const { expression->export_locals(scope); while_true->export_locals(scope); }
		virtual void compile(Codegen& codegen) { codegen.compile(*this); }
	};
}
}

#endif /* end of include guard: NODE_H_WML097FE */

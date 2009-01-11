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
		
		virtual void export_locals(Scope::LocalList&) const {}
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
		std::string m_String;
		Type m_Type;
		
		Literal(const std::string& str, Type type) : m_String(str), m_Type(type) {}
	};
	
	struct Identifier : Node {
		std::string name;
		
		Identifier(const std::string& name) : name(name) {}
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
		
		virtual void export_locals(Scope::LocalList& list) const {
			for each (iter, nodes) { (*iter)->export_locals(list); }
		}
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
	};
	
	struct Assignment : Node {
		RefPtr<Identifier> identifier;
		RefPtr<Node> expression;
		Assignment(RefPtr<Identifier> ident, RefPtr<Node> expr) : identifier(ident), expression(expr) {}
		virtual void export_locals(Scope::LocalList& list) const { list.add(identifier->name); expression->export_locals(list); }
	};
	
	struct Condition : Node {
		RefPtr<Node> expression;
		Condition(RefPtr<Node> expr) : expression(expr) {}
		virtual void export_locals(Scope::LocalList& list) const { expression->export_locals(list); }
	};
	
	struct IfCondition : public Condition {
		RefPtr<Node> if_true;
		IfCondition(RefPtr<Node> expr, RefPtr<Node> if_true) : Condition(expr), if_true(if_true) {}
		virtual void export_locals(Scope::LocalList& list) const { Condition::export_locals(list); if_true->export_locals(list); }
	};
	
	struct IfElseCondition : public IfCondition {
		RefPtr<Node> if_false;
		IfElseCondition(RefPtr<Node> expr, RefPtr<Node> if_true, RefPtr<Node> if_false) : IfCondition(expr, if_true), if_false(if_false) {}
		virtual void export_locals(Scope::LocalList& list) const { IfCondition::export_locals(list); if_false->export_locals(list); }
	};
	
	struct Call : Node {
		RefPtr<Node> object;
		RefPtr<Sequence> arguments;
		Call(RefPtr<Node> obj, RefPtr<Sequence> args = NULL) : object(obj), arguments(args) {}
		virtual void export_locals(Scope::LocalList& list) const { object->export_locals(list); if (arguments) arguments->export_locals(list); }
	};
	
	struct Send : Node {
		RefPtr<Node> self;
		RefPtr<Node> message;
		Send(RefPtr<Node> self, RefPtr<Node> message) : self(self), message(message) {}
		virtual void export_locals(Scope::LocalList& list) const { self->export_locals(list); message->export_locals(list); }
	};
	
	struct Loop : Node {
		RefPtr<Node> expression;
		RefPtr<Node> while_true;
		Loop(RefPtr<Node> expression, RefPtr<Node> while_true) : expression(expression), while_true(while_true) {}
		virtual void export_locals(Scope::LocalList& list) const { expression->export_locals(list); while_true->export_locals(list); }
	};
}
}

#endif /* end of include guard: NODE_H_WML097FE */

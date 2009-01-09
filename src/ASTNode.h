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
		
		virtual int count_locals() const { return 0; }
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
		
		virtual int count_locals() const {
			int sum = 0;
			for each (iter, nodes) { sum += (*iter)->count_locals(); }
			return sum;
		}
	};
	
	struct Scope : Node {
		std::list<RefPtr<Identifier>> arguments;
		RefPtr<Sequence> sequence;
		
		Scope() : sequence(new Sequence) {}
		template <typename... T>
		Scope(const RefPtr<Node>& first, const T&... args) : sequence(new Sequence) { add(first, args...); }
		void add() {}
		template <typename... T>
		void add(const RefPtr<Node>& node, const T&... args) { sequence->add(node, args...); }
	};
	
	struct Assignment : Node {
		RefPtr<Identifier> identifier;
		RefPtr<Node> expression;
		Assignment(RefPtr<Identifier> ident, RefPtr<Node> expr) : identifier(ident), expression(expr) {}
		virtual int count_locals() const { return 1 + expression->count_locals(); }
	};
	
	struct Condition : Node {
		RefPtr<Node> expression;
		Condition(RefPtr<Node> expr) : expression(expr) {}
		virtual int count_locals() const { return expression->count_locals(); }
	};
	
	struct IfCondition : public Condition {
		RefPtr<Node> if_true;
		IfCondition(RefPtr<Node> expr, RefPtr<Node> if_true) : Condition(expr), if_true(if_true) {}
		virtual int count_locals() const { return Condition::count_locals() + if_true->count_locals(); }
	};
	
	struct IfElseCondition : public IfCondition {
		RefPtr<Node> if_false;
		IfElseCondition(RefPtr<Node> expr, RefPtr<Node> if_true, RefPtr<Node> if_false) : IfCondition(expr, if_true), if_false(if_false) {}
		virtual int count_locals() const { return IfCondition::count_locals() + if_false->count_locals(); }
	};
	
	struct Call : Node {
		RefPtr<Node> object;
		RefPtr<Sequence> arguments;
		Call(RefPtr<Node> obj, RefPtr<Sequence> args = NULL) : object(obj), arguments(args) {}
		virtual int count_locals() const { return object->count_locals() + (arguments ? arguments->count_locals() : 0); }
	};
	
	struct Send : Node {
		RefPtr<Node> self;
		RefPtr<Node> message;
		Send(RefPtr<Node> self, RefPtr<Node> message) : self(self), message(message) {}
		virtual int count_locals() const { return self->count_locals() + message->count_locals(); }
	};
	
	struct Loop : Node {
		RefPtr<Node> expression;
		RefPtr<Node> while_true;
		Loop(RefPtr<Node> expression, RefPtr<Node> while_true) : expression(expression), while_true(while_true) {}
		virtual int count_locals() const { return expression->count_locals() + while_true->count_locals(); }
	};
}
}

#endif /* end of include guard: NODE_H_WML097FE */

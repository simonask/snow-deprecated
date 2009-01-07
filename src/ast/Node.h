#ifndef NODE_H_WML097FE
#define NODE_H_WML097FE

#include "Basic.h"
#include "Codegen.h"
#include <string>
#include <list>

namespace snot {
namespace ast {
	class Node {
	public:
		virtual ~Node() {}
		
		virtual void realize(Codegen&) {}
		
		template <class T>
		bool is_a() const { return dynamic_cast<T*>(this) != NULL; }
	};
	
	class Literal : public Node {
	public:
		enum Type {
			STRING_TYPE,
			INTEGER_TYPE,
			INTEGER_DEC_TYPE = INTEGER_TYPE,
			INTEGER_HEX_TYPE,
			INTEGER_BIN_TYPE,
			FLOAT_TYPE,
		};
	protected:
		std::string m_String;
		Type m_Type;
	public:
		Literal(const std::string& str, Type type) : m_String(str), m_Type(type) {}
	};
	
	class Identifier : public Node {
	protected:
		std::string m_Name;
	public:
		Identifier(const std::string& name) : m_Name(name) {}
	};
	
	class Sequence : public Node {
	protected:
		std::list<RefPtr<Node>> m_Nodes;
	public:
		void add(RefPtr<Node> node) { m_Nodes.push_back(node); }
		std::list<RefPtr<Node>>& nodes() { return m_Nodes; }
		const std::list<RefPtr<Node>>& nodes() const { return m_Nodes; }
		virtual void realize(Codegen&);
	};
	
	class Scope : public Node {
	protected:
		std::list<RefPtr<Identifier>> m_Arguments;
		RefPtr<Sequence> m_Sequence;
	public:
		void add(RefPtr<Node> node) { m_Sequence->add(node); }
	};
	
	class Assignment : public Node {
	protected:
		RefPtr<Identifier> m_Identifier;
		RefPtr<Node> m_Expression;
	public:
		Assignment(RefPtr<Identifier> ident, RefPtr<Node> expr) : m_Identifier(ident), m_Expression(expr) {}
	};
	
	class Condition : public Node {
	protected:
		RefPtr<Node> m_Expression;
	public:
		Condition(RefPtr<Node> expr) : m_Expression(expr) {}
		virtual void realize(Codegen&);
	};
	
	class IfCondition : public Condition {
	protected:
		RefPtr<Node> m_IfTrue;
	public:
		IfCondition(RefPtr<Node> expr, RefPtr<Node> if_true) : Condition(expr), m_IfTrue(if_true) {}
		virtual void realize(Codegen&);
	};
	
	class IfElseCondition : public IfCondition {
	protected:
		RefPtr<Node> m_IfFalse;
	public:
		IfElseCondition(RefPtr<Node> expr, RefPtr<Node> if_true, RefPtr<Node> if_false) : IfCondition(expr, if_true), m_IfFalse(if_false) {}
		virtual void realize(Codegen&);
	};
	
	class Call : public Node {
	protected:
		RefPtr<Node> m_Object;
		RefPtr<Sequence> m_Arguments;
	public:
		Call(RefPtr<Node> obj, RefPtr<Sequence> args) : m_Object(obj), m_Arguments(args) {}
		virtual void realize(Codegen&);
	};
	
	class Send : public Node {
	protected:
		RefPtr<Node> m_Self;
		RefPtr<Node> m_Method;
		RefPtr<Sequence> m_Arguments;
	public:
		Send(RefPtr<Node> self, RefPtr<Node> method, RefPtr<Sequence> args) : m_Self(self), m_Method(method), m_Arguments(args) {}
		virtual void realize(Codegen&);
	};
}
}

#endif /* end of include guard: NODE_H_WML097FE */

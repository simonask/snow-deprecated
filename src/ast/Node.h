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
		
		virtual void realize(Codegen&) = 0;
	};
	
	class Literal : public Node { };
	
	class LiteralString : public Literal {
	protected:
		std::string m_String;
	public:
		LiteralString(const std::string& str) : m_String(str) {}
	};
	
	class LiteralInteger : public Literal {
	protected:
		
	};
	
	class Sequence : public Node {
	protected:
		std::list<Node*> m_Nodes;
		
		void add(Node* node) { m_Nodes.push_back(node); }
	};
	
	class Condition : public Node {
	protected:
		Node* m_Expression;
	};
	
	class IfCondition : public Condition {
	protected:
		Node* m_IfTrue;
	};
	
	class IfElseCondition : public IfCondition {
	protected:
		Node* m_Else;
	};
}
}

#endif /* end of include guard: NODE_H_WML097FE */

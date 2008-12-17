#ifndef AST_H_ULB90X9M
#define AST_H_ULB90X9M

#include "Codegen.h"

namespace snot {
namespace ast {

class Node {
public:
	enum Child {
		LEFT = 0,
		RIGHT,
		NUM_CHILDREN
	};
protected:
	Node* m_Children[2];
public:
	Node() { m_Children[LEFT] = NULL; m_Children[RIGHT] = NULL; }
	virtual void emit(Codegen&) const = 0;
	void set(Child c, Node* node) { m_Children[c] = node; }
	Node* child(Child c) const { return m_Children[c]; }
	Node* child() const { return child(LEFT); } /* default for unary nodes */
};

class ScopeNode : public Node {
public:
	void emit(Codegen&) const;
};

}
}

#endif /* end of include guard: AST_H_ULB90X9M */

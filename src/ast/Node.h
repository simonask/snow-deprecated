#ifndef NODE_H_WML097FE
#define NODE_H_WML097FE

#include "Basic.h"

namespace snot {
namespace ast {
	template <int C>
	class Node {
	protected:
		Node* m_Children[C];
	};
}
}

#endif /* end of include guard: NODE_H_WML097FE */

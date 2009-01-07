#include "Node.h"

namespace snot {
namespace ast {
	void Sequence::realize(Codegen& m) {
		for each (iter, m_Nodes) {
			(*iter)->realize(m);
		}
	}
	
	void Assignment::realize(Codegen& m) {
//		auto local = m.local(m_Identifier->name());
		m_Expression->realize(m);
	}
	
	void Call::realize(Codegen&) {}
	
	void Send::realize(Codegen&) {}
}
}
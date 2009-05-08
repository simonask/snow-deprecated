#include "Handle.h"
#include <pthread.h>
#include <set>

namespace snow {
	VALUE StackVariable::s_Null = NULL;

	/// HandleScope ///

	HandleScope::HandleScope() : m_Destructing(false), m_LastVariable(NULL) {
		m_Previous = current();
		set_current(this);
	}

	HandleScope::~HandleScope() {
		ASSERT(!m_Destructing);
		m_Destructing = true;
		StackVariable* var = m_LastVariable;
		while (var) {
			var->~StackVariable();
			var = var->m_Previous;
		}
		set_current(m_Previous);
	}

	void HandleScope::add(StackVariable* var) {
		ASSERT(!m_Destructing);
		var->m_Previous = m_LastVariable;
		m_LastVariable = var;
	}

	void HandleScope::remove(StackVariable* var) {
		if (m_Destructing) return;
		StackVariable* current = m_LastVariable;
		StackVariable* next = NULL;
		while (current) {
			if (current == var) {
				if (next)
					next->m_Previous = current->m_Previous;
				else
					m_LastVariable = current->m_Previous;
				break;
			}
			next = current;
			current = current->m_Previous;
		}
	}

	static HandleScope* current_handle_scope = NULL;

	HandleScope* HandleScope::current() {
		return current_handle_scope;
	}

	void HandleScope::set_current(HandleScope* scope) {
		current_handle_scope = scope;
	}
}

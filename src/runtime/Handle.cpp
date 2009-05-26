#include "Handle.h"
#include "base/ThreadLocal.h"
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
			StackVariable* previous = var->m_Previous;
			ASSERT(previous != var);
			var->~StackVariable();
			var = previous;
		}
		set_current(m_Previous);
	}

	void HandleScope::add(StackVariable* var) {
		ASSERT(!m_Destructing);
		ASSERT(var != m_LastVariable);
		var->m_Previous = m_LastVariable;
		m_LastVariable = var;
	}

	void HandleScope::remove(StackVariable* obsolete) {
		if (m_Destructing) return;
		ASSERT(obsolete != NULL);
		StackVariable* current = m_LastVariable;
		StackVariable* after_obsolete = NULL;
		while (current) {
			if (current->m_Previous == obsolete)
			{
				after_obsolete = current;
				break;
			}
			ASSERT(current != current->m_Previous);
			current = current->m_Previous;
		}

		if (after_obsolete) {
			after_obsolete->m_Previous = obsolete->m_Previous;
		} else {
			m_LastVariable = obsolete->m_Previous;
		}
	}

	static ThreadLocal<HandleScope*> current_handle_scope = NULL;

	HandleScope* HandleScope::current() {
		return current_handle_scope;
	}

	void HandleScope::set_current(HandleScope* scope) {
		current_handle_scope = scope;
	}
}

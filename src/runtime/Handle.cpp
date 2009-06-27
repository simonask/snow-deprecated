#include "Handle.h"
#include <pthread.h>
#include <set>

namespace snow {
	/// HandleScope ///

	HandleScope::HandleScope() : m_Destructing(false), m_LastHandle(NULL), m_LastLocal(NULL) {
		m_Previous = current();
		set_current(this);
	}

	HandleScope::~HandleScope() {
		ASSERT(!m_Destructing);
		m_Destructing = true;
		BasicLocal* var = m_LastLocal;
		while (var) {
			BasicLocal* previous = var->m_Previous;
			ASSERT(previous != var);
			var->~BasicLocal();
			var = previous;
		}
		set_current(m_Previous);
	}

	void HandleScope::add(BasicLocal* var) {
		ASSERT(!m_Destructing);
		ASSERT(var != m_LastLocal);
		var->m_Previous = m_LastLocal;
		m_LastLocal = var;
	}

	void HandleScope::remove(BasicLocal* obsolete) {
		if (m_Destructing) return;
		ASSERT(obsolete != NULL);
		BasicLocal* current = m_LastLocal;
		BasicLocal* after_obsolete = NULL;
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
			m_LastLocal = obsolete->m_Previous;
		}
	}
	
	void HandleScope::add(Handle<void>* handle) {
		ASSERT(!m_Destructing);
		ASSERT(handle != m_LastHandle);
		handle->m_Previous = m_LastHandle;
		m_LastHandle = handle;
	}
	
	void HandleScope::remove(Handle<void>* obsolete) {
		if (m_Destructing) return;
		ASSERT(obsolete != NULL);
		Handle<void>* current = m_LastHandle;
		Handle<void>* after_obsolete = NULL;
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
			m_LastHandle = obsolete->m_Previous;
		}
	}

	static ThreadLocal<HandleScope*> current_handle_scope = NULL;

	HandleScope* HandleScope::current() {
		return current_handle_scope;
	}

	ThreadLocal<HandleScope*>& HandleScope::all_current() {
		return current_handle_scope;
	}

	void HandleScope::set_current(HandleScope* scope) {
		current_handle_scope = scope;
	}
}

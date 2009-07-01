#include "Handle.h"
#include <pthread.h>
#include <set>

namespace snow {
	/// HandleScope ///

	HandleScope::HandleScope() : m_Destructing(false), m_LastHandle(NULL), m_LastLocal(NULL), m_LastStackProtector(NULL) {
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
	
	template <typename T>
	void HandleScope::add_implicit_link(T*& tail, T* obj) {
		ASSERT(obj != tail);
		obj->m_Previous = tail;
		tail = obj;
	}
	
	template <typename T>
	void HandleScope::remove_implicit_link(T*& tail, T* obsolete) {
		ASSERT(obsolete != NULL);
		T* current = tail;
		T* after_obsolete = NULL;
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
			tail = obsolete->m_Previous;
		}
	}

	void HandleScope::add(BasicLocal* var) {
		ASSERT(!m_Destructing);
		add_implicit_link(m_LastLocal, var);
	}

	void HandleScope::remove(BasicLocal* obsolete) {
		if (m_Destructing) return;
		remove_implicit_link(m_LastLocal, obsolete);
	}
	
	void HandleScope::add(Handle<void>* handle) {
		ASSERT(!m_Destructing);
		add_implicit_link(m_LastHandle, handle);
	}
	
	void HandleScope::remove(Handle<void>* obsolete) {
		if (m_Destructing) return;
		remove_implicit_link(m_LastHandle, obsolete);
	}
	
	void HandleScope::add(StackProtector* sp) {
		ASSERT(!m_Destructing);
		ASSERT(sp != m_LastStackProtector);
		add_implicit_link(m_LastStackProtector, sp);
	}
	
	void HandleScope::remove(StackProtector* sp) {
		if (m_Destructing) return;
		remove_implicit_link(m_LastStackProtector, sp);
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

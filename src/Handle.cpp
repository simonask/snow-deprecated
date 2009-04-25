#include "Handle.h"
#include <pthread.h>
#include <set>

namespace snow {
	ValueHandle::ValueHandle(VALUE val) : m_Value(val), m_Scope(NULL) {
		HandleScope::List::iterator scope_iter = HandleScope::list().begin();
		ASSERT(scope_iter != HandleScope::list().end() && "Handle outside scope!");

		m_Scope = *scope_iter;
		m_Scope->add(this);
	}
	
	ValueHandle::ValueHandle(const ValueHandle& other) : m_Value(other.m_Value), m_Scope(NULL) {
		HandleScope::List::iterator scope_iter = HandleScope::list().begin();
		ASSERT(scope_iter != HandleScope::list().end() && "Handle outside scope!");

		m_Scope = *scope_iter;
		m_Scope->add(this);
	}
	
	ValueHandle::~ValueHandle() {
		m_Scope->remove(this);
	}
	
	/// HandleScope ///

	HandleScope::HandleScope() : m_Destructing(false) {
		m_Iterator = list().insert(list().end(), this);
	}

	HandleScope::~HandleScope() {
		m_Destructing = true;
		for each (iter, m_Handles) {
			(*iter)->~ValueHandle();
		}
		for each (iter, m_Locals) {
			(*iter)->~AbstractLocal();
		}
		list().erase(m_Iterator);
	}

	ValueHandle::List::iterator HandleScope::add(ValueHandle* handle) {
		ASSERT(!m_Destructing);
		return m_Handles.insert(m_Handles.end(), handle);
	}

	void HandleScope::remove(ValueHandle* handle) {
		if (m_Destructing) return;
		for (auto iter = m_Handles.begin(); iter != m_Handles.end(); ++iter) {
			if (*iter == handle) {
				iter = m_Handles.erase(iter);
			}
		}
	}

	void HandleScope::add_local(AbstractLocal* local) {
		ASSERT(!m_Destructing);
		m_Locals.insert(m_Locals.end(), local);
	}

	void HandleScope::remove_local(AbstractLocal* local) {
		if (m_Destructing) return;
		for (auto iter = m_Locals.begin(); iter != m_Locals.end(); ++iter) {
			if (*iter == local) {
				iter = m_Locals.erase(iter);
			}
		}
	}

	HandleScope::List& HandleScope::list() {
		static HandleScope::List the_list;
		return the_list;
	}

	HandleScope& HandleScope::current() {
		return *list().back();
	}
}

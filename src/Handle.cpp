#include "Handle.h"
#include <pthread.h>
#include <set>

namespace snow {
	static std::set<ValueHandle*>& _permanents() {
		static std::set<ValueHandle*> _set;
		return _set;
	}
	
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
	
	void ValueHandle::set_permanent() {
		_permanents().insert(this);
	}
	
	/// HandleScope ///

	HandleScope::HandleScope() {
		m_Iterator = list().insert(list().end(), this);
	}

	HandleScope::~HandleScope() {
		for each (iter, m_Handles) {
			(*iter)->~ValueHandle();
		}
		list().erase(m_Iterator);
	}

	ValueHandle::List::iterator HandleScope::add(ValueHandle* handle) {
		return m_Handles.insert(m_Handles.end(), handle);
	}

	void HandleScope::remove(ValueHandle* handle) {
		for (auto iter = m_Handles.begin(); iter != m_Handles.end(); ++iter) {
			if (*iter == handle) {
				iter = m_Handles.erase(iter);
			}
		}
	}

	HandleScope::List& HandleScope::list() {
		static HandleScope::List the_list;
		return the_list;
	}
}

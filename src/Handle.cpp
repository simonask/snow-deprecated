#include "Handle.h"
#include <pthread.h>
#include <set>

namespace snow {
	static std::set<ValueHandle*>& _permanents() {
		static std::set<ValueHandle*> _set;
		return _set;
	}
	
	ValueHandle::ValueHandle(VALUE val) : m_Value(val) {
		m_Iterator = list().insert(list().end(), this);
		ASSERT(*m_Iterator == this);
	}
	
	ValueHandle::ValueHandle(const ValueHandle& other) : m_Value(other.m_Value) {
		m_Iterator = list().insert(list().end(), this);
		ASSERT(*m_Iterator == this);
	}
	
	ValueHandle::~ValueHandle() {
		if (_permanents().find(this) == _permanents().end())
			list().erase(m_Iterator);
	}
	
	void ValueHandle::set_permanent() {
		_permanents().insert(this);
	}
	
	ValueHandle::List& ValueHandle::list() {
		static List the_list;
		return the_list;
	}
}
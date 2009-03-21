#include "Handle.h"
#include "Object.h"
#include <pthread.h>

namespace snow {
	static ValueHandle*& last_handle_holder() {
		static ValueHandle* last_handle = NULL;
		return last_handle;
	}
	
	static inline ValueHandle* last_handle() {
		return last_handle_holder();
	}
	
	static inline void set_last_handle(ValueHandle* handle) {
		last_handle_holder() = handle;
	}
	
	ValueHandle::ValueHandle(VALUE val) : m_Value(val), m_Previous(NULL), m_Next(NULL) {
		set_previous(last_handle());
		//debug("SETTING LAST HANDLE (new): 0x%llx (previous: 0x%llx, next: 0x%llx)", this, this->previous(), this->next());
		set_last_handle(this);
		if (previous())
			previous()->set_next(this);
	}
	
	ValueHandle::~ValueHandle() {
		//debug("destroying 0x%llx (previous: 0x%llx, next: 0x%llx)", this, previous(), next());
		if (this == last_handle()) {
			set_last_handle(m_Previous);
			if (previous())
				previous()->set_next(NULL);
		} else if (previous()) {
			previous()->set_next(next());
		}
		if (next()) {
			next()->set_previous(previous());
		}
	}
	
	void ValueHandle::set_previous(ValueHandle* other) {
		ASSERT(other != this);
		m_Previous = other;
	}
	
	void ValueHandle::set_next(ValueHandle* other) {
		ASSERT(other != this);
		m_Next = other;
	}
	
	ValueHandle* ValueHandle::last() {
		return last_handle();
	}
}
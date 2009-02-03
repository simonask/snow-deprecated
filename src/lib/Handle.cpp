#include "Handle.h"
#include "Object.h"
#include <pthread.h>

namespace snow {
	static Handle* last_handle = NULL;
	
	Handle::Handle(VALUE value) : m_Value(value), m_Next(NULL) {
		static bool initialized = false;
		if (!initialized) {
			last_handle = NULL;
		}
		
		m_Previous = last_handle;
		last_handle = this;
		if (m_Previous)
			m_Previous->m_Next = this;
	}
	
	Handle::~Handle() {
		if (m_Previous)
			m_Previous->m_Next = m_Next;
		if (m_Next)
			m_Next->m_Previous = m_Previous;
		last_handle = m_Previous;
	}
	
	Handle* Handle::last() {
//		return (Handle*)pthread_getspecific(handle_key);
		return last_handle;
	}
	
	Handle::operator Object*() const {
		return cast<Object>();
	}
	
	Object* Handle::operator->() {
		return cast<Object>();
	}
	
	const Object* Handle::operator->() const {
		return cast<const Object>();
	}
}
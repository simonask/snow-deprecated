#include "Handle.h"
#include "Object.h"
#include <pthread.h>

namespace snow {
	static ValueHandle* last_handle = NULL;
	
	ValueHandle::ValueHandle(VALUE value) : m_Value(value), m_Next(NULL) {
		static bool initialized = false;
		if (!initialized) {
			last_handle = NULL;
		}
		
		m_Previous = last_handle;
		last_handle = this;
		if (m_Previous)
			m_Previous->m_Next = this;
	}
	
	ValueHandle::~ValueHandle() {
		if (m_Previous)
			m_Previous->m_Next = m_Next;
		if (m_Next)
			m_Next->m_Previous = m_Previous;
		last_handle = m_Previous;
	}
	
	ValueHandle* ValueHandle::last() {
//		return (ValueHandle*)pthread_getspecific(handle_key);
		return last_handle;
	}
}
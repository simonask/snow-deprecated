#include "lib/Exception.h"
#include <list>

namespace snow {
	// TODO: Thread-safe exceptions

	ExceptionHandler* ExceptionHandler::s_Current = NULL;

	ExceptionHandler::ExceptionHandler() : m_HandleScope(HandleScope::current()), m_Exception(NULL), m_Previous(NULL) {
		m_Previous = s_Current;
		s_Current = this;
	}
	
	ExceptionHandler::~ExceptionHandler() {
		s_Current = m_Previous;
	}

	bool ExceptionHandler::_returning_directly() volatile {
		// Must always return false!
		return false;
	}

	bool ExceptionHandler::_returning_from_exception() volatile {
		s_Current = m_Previous;

		// Must always return true!
		return true;
	}

	void throw_exception(VALUE ex) {
		ASSERT(ExceptionHandler::s_Current);
		ExceptionHandler::s_Current->m_Exception = ex;
		// Call stack destructors
		for each_reverse(iter, HandleScope::list()) {
			if (*iter == &ExceptionHandler::s_Current->m_HandleScope)
				break;
			for each_reverse(handle_iter, (*iter)->handles()) {
				(*handle_iter)->~ValueHandle();
			}
			(*iter)->~HandleScope();
		}
		longjmp(ExceptionHandler::s_Current->m_JmpBuf, 1);
	}
}


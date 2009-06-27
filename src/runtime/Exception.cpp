#include "runtime/Exception.h"
#include "gc/MemoryManager.h"
#include "runtime/Runtime.h"
#include "runtime/StackFrame.h"

#include <list>
#include <sstream>

namespace snow {
	// TODO: Thread-safe exceptions

	ExceptionHandler* ExceptionHandler::s_Current = NULL;

	ExceptionHandler::ExceptionHandler() : m_HandleScope(*HandleScope::current()), m_Exception(NULL), m_StackFrame(NULL), m_ThrowingStackFrame(NULL), m_Previous(NULL), m_StackTrace(NULL) {
	}
	
	ExceptionHandler::~ExceptionHandler() {
		s_Current = m_Previous;

		// When the destructor is called, it means this handler is no longer active, so we can safely delete any stack trace.
		if (!s_Current) {
			// XXX: Possible leak here!
			size_t i = 0;
			while (m_StackTrace && m_StackTrace[i]) {
				delete[] m_StackTrace[i];
				++i;
			}
			delete[] m_StackTrace;
			m_StackTrace = NULL;
		}
	}

	bool ExceptionHandler::_returning_directly() {
		m_StackFrame = get_current_stack_frame();
		m_Previous = s_Current;
		s_Current = this;
		// Must always return false!
		return false;
	}

	bool ExceptionHandler::_returning_from_exception() {
		s_Current = m_Previous;
		set_current_stack_frame(m_StackFrame);

		if (s_Current) {
			s_Current->m_StackTrace = m_StackTrace;
		}

		// Must always return true!
		return true;
	}

	void throw_exception(const Value& ex) {
		ASSERT(ExceptionHandler::s_Current);
		ExceptionHandler::s_Current->m_Exception = ex;
		ExceptionHandler::s_Current->m_ThrowingStackFrame = get_current_stack_frame();

		// Call stack destructors
		HandleScope* handle_scope = HandleScope::current();
		while (handle_scope && handle_scope != &ExceptionHandler::s_Current->m_HandleScope) {
			handle_scope->~HandleScope();
			handle_scope = handle_scope->previous();
		}

		if (!ExceptionHandler::s_Current->m_StackTrace) {
			// Create stack trace
			uintx stack_depth = 0;
			StackFrame* frame = get_current_stack_frame();

			while (frame) { ++stack_depth; frame = frame->previous; }

			char** trace_list = new(kMalloc) char*[stack_depth+1];
			trace_list[stack_depth] = NULL;

			frame = get_current_stack_frame();
			uintx i = 0;
			while (frame) {
				std::stringstream ss;
				ss << frame->file << ":" << frame->line;
				size_t width = ss.str().length();
				while (width < 40) {
					ss << " ";
					++width;
				}

				ss << frame->funcname; /*<< "(";
				for (size_t j = 0; j < frame->num_args; ++j) {
					ss << std::hex << frame->args[j];
					if (j != frame->num_args-1)
						ss << ", ";
				}
				ss << ")";
				*/

				std::string str = ss.str();
				size_t len = str.length();
				char* cstr = new(kMalloc) char[len+1];
				memcpy(cstr, str.c_str(), len+1);
				trace_list[i] = cstr;

				frame = frame->previous;
				++i;
			}

			ExceptionHandler::s_Current->m_StackTrace = trace_list;
		}

		longjmp(ExceptionHandler::s_Current->m_JmpBuf, 1);
	}
}


#include "lib/Exception.h"
#include "MemoryManager.h"
#include <list>
#include "lib/Runtime.h"

namespace snow {
	// TODO: Thread-safe exceptions

	ExceptionHandler* ExceptionHandler::s_Current = NULL;

	ExceptionHandler::ExceptionHandler() : m_HandleScope(HandleScope::current()), m_Exception(NULL), m_StackFrame(NULL), m_Previous(NULL), m_StackTrace(NULL) {
	}
	
	ExceptionHandler::~ExceptionHandler() {
		s_Current = m_Previous;

		// When the destructor is called, it means this handler is no longer active, so we can safely delete any stack trace.
		if (!s_Current) {
			// XXX: Possible leak here!
			size_t i = 0;
			while (m_StackTrace && m_StackTrace[i]) {
				free(m_StackTrace[i]);
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

		if (!ExceptionHandler::s_Current->m_StackTrace) {
			// Create stack trace
			uint64_t stack_depth = 0;
			StackFrame* frame = get_current_stack_frame();

			while (frame) { ++stack_depth; frame = frame->previous; }

			char** trace_list = new(kMalloc) char*[stack_depth+1];
			trace_list[stack_depth] = NULL;

			frame = get_current_stack_frame();
			uint64_t i = 0;
			while (frame) {
				std::stringstream ss;
				ss << frame->file << ":" << frame->line;
				size_t width = ss.str().length();
				while (width < 40) {
					ss << " ";
					++width;
				}

				ss << frame->funcname << "(";
				for (size_t j = 0; j < frame->num_args; ++j) {
					ss << std::hex << frame->args[j];
					if (j != frame->num_args-1)
						ss << ", ";
				}
				ss << ")";

				std::string str = ss.str();
				size_t len = str.length();
				char* cstr = (char*)malloc(len+1);
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


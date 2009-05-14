#ifndef EXCEPTION_H_RTDCA32
#define EXCEPTION_H_RTDCA32

#include <setjmp.h>
#include "base/Basic.h"
#include "Handle.h"
#include "Runtime.h"

namespace snow {
	void throw_exception(VALUE ex);

	class ExceptionHandler {
		friend void throw_exception(VALUE);
	private:
		HandleScope& m_HandleScope;
		VALUE m_Exception;
		StackFrame* m_StackFrame;
		ExceptionHandler* m_Previous;
		char** m_StackTrace;

		static ExceptionHandler* s_Current;
	public:
		jmp_buf m_JmpBuf;

		ExceptionHandler();
		~ExceptionHandler();
		jmp_buf& jump_buffer() { return m_JmpBuf; }
		VALUE exception() const { return m_Exception; }
		char const* const* stack_trace() const { return m_StackTrace; }
		const StackFrame* stack_frame() const { return m_StackFrame; }

		// Internal, used by TRY_CATCH macro
		bool _returning_from_exception();
		bool _returning_directly();
	};

#define TRY_CATCH(handler) !((setjmp(handler.m_JmpBuf) && handler._returning_from_exception()) || handler._returning_directly())
}

#endif // EXCEPTION_H_RTDCA32


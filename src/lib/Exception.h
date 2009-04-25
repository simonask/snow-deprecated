#ifndef EXCEPTION_H_RTDCA32
#define EXCEPTION_H_RTDCA32

#include <setjmp.h>
#include "Basic.h"
#include "Handle.h"
#include "Runtime.h"

namespace snow {
	void throw_exception(VALUE ex);

	class ExceptionHandler {
		friend void throw_exception(VALUE);
	private:
		HandleScope& m_HandleScope;
		jmp_buf m_JmpBuf;
		VALUE m_Exception;
		StackFrame* m_StackFrame;

		static ExceptionHandler* s_Current;
		ExceptionHandler* m_Previous;
	public:
		ExceptionHandler();
		~ExceptionHandler();
		jmp_buf& jump_buffer() { return m_JmpBuf; }
		VALUE exception() const { return m_Exception; }

		// Internal, used by TRY_CATCH macro
		bool _returning_from_exception() volatile;
		bool _returning_directly() volatile;
	};

#define TRY_CATCH(handler) !((setjmp(handler.jump_buffer()) && handler._returning_from_exception()) || handler._returning_directly())
//#define TRY_CATCH(handler) !(setjmp(handler.jump_buffer()))
}

#endif // EXCEPTION_H_RTDCA32


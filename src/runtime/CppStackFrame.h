#ifndef CPPSTACKFRAME_H_CVWJ456
#define CPPSTACKFRAME_H_CVWJ456

#include "StackFrame.h"
#include "Handle.h"
#include "runtime/Runtime.h"

namespace snow {
	class CppStackFrame {
	private:
		StackFrame m_Frame;		
		HandleScope m_HandleScope;
	public:
		CppStackFrame(VALUE self, size_t num_args, VALUE* args, const char* file, size_t line, const char* func);
		~CppStackFrame();
	};

	inline CppStackFrame::CppStackFrame(VALUE self, size_t num_args, VALUE* args, const char* file, size_t line, const char* func) {
		m_Frame.temporaries = NULL;
		m_Frame.num_temporaries = 0;
		enter_scope(NULL, &m_Frame);
		m_Frame.self = self;
		m_Frame.args = args;
		m_Frame.num_args = num_args;
		m_Frame.file = file;
		m_Frame.line = line; 
		m_Frame.funcname = func;
		m_Frame.it = num_args > 0 ? args[0] : NULL;
		m_Frame.locals = NULL;
	}

	inline CppStackFrame::~CppStackFrame() {
		leave_scope();
	}
}

#define SCOPE(self, num_args, args) snow::CppStackFrame _cpp_stack_frame(self, num_args, args, __FILE__, __LINE__, __func__)
#define NORMAL_SCOPE() SCOPE(self, num_args, args)

#endif


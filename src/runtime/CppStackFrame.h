#ifndef CPPSTACKFRAME_H_CVWJ456
#define CPPSTACKFRAME_H_CVWJ456

#include "StackFrame.h"
#include "Handle.h"
#include "runtime/Runtime.h"
#include "runtime/Arguments.h"

namespace snow {
	class CppStackFrame {
	private:
		StackFrame m_Frame;		
		HandleScope m_HandleScope;
	public:
		CppStackFrame(const Value& self, const Arguments&, const char* file, size_t line, const char* func);
		~CppStackFrame();
	};

	inline CppStackFrame::CppStackFrame(const Value& self, const Arguments& args, const char* file, size_t line, const char* func) {
		m_Frame.temporaries = NULL;
		m_Frame.num_temporaries = 0;
		enter_scope(NULL, &m_Frame);
		m_Frame.self = self.value();
		m_Frame.args = args.data;
		m_Frame.num_args = args.size;
		m_Frame.file = file;
		m_Frame.line = line; 
		m_Frame.funcname = func;
		m_Frame.it = args.size > 0 ? args.data[0] : NULL;
		m_Frame.locals = NULL;
	}

	inline CppStackFrame::~CppStackFrame() {
		leave_scope();
	}
}

#define SCOPE(self, args) snow::CppStackFrame _cpp_stack_frame(self, args, __FILE__, __LINE__, __func__)
#define NORMAL_SCOPE() SCOPE(self, args)

#endif


#ifndef FUNCTION_H_ZAXPBYJ7
#define FUNCTION_H_ZAXPBYJ7

#include "Object.h"

namespace snot {
	typedef VALUE(*FunctionPtr)(VALUE self, uint64_t num_args, VALUE* args);
	
	class Function : public Object {
	private:
		FunctionPtr m_Ptr;
	public:
		Function(FunctionPtr ptr) : Object(NULL), m_Ptr(ptr) {}
		VALUE va_call(VALUE self, uint64_t num_args, va_list&);

		FunctionPtr function() const { return m_Ptr; }
	};
}

#endif /* end of include guard: FUNCTION_H_ZAXPBYJ7 */

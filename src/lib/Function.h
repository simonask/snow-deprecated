#ifndef FUNCTION_H_ZAXPBYJ7
#define FUNCTION_H_ZAXPBYJ7

#include "Object.h"

namespace snow {
	typedef VALUE(*FunctionPtr)(VALUE self, uint64_t num_args, VALUE* args);
	
	Object* function_prototype();
	
	class Function : public Object {
	private:
		FunctionPtr m_Ptr;
	public:
		Function(FunctionPtr ptr) : Object(function_prototype()), m_Ptr(ptr) {}
		VALUE va_call(VALUE self, uint64_t num_args, va_list&);
		VALUE copy() const { return value((Object*)new Function(*this)); }

		FunctionPtr function() const { return m_Ptr; }
	};
}

#endif /* end of include guard: FUNCTION_H_ZAXPBYJ7 */

#include "Function.h"

namespace snot {
	VALUE Function::va_call(VALUE self, uint64_t num_args, va_list& ap) {
		VALUE args[num_args];
		for (uint64_t i = 0; i < num_args; ++i) {
			args[i] = va_arg(ap, VALUE);
		}
		// TODO: copy?
		return m_Ptr(self, num_args, num_args ? args : NULL);
	}
}
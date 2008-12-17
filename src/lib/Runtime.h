#ifndef RUNTIME_H_RUMHEGGF
#define RUNTIME_H_RUMHEGGF

#include "Value.h"

namespace snot {
	VALUE create_object(Object* prototype);
	VALUE call(VALUE obj);
	VALUE send(VALUE obj, const char* message, uint64_t num_args, ...);
}


#endif /* end of include guard: RUNTIME_H_RUMHEGGF */

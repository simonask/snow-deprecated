#include "Object.h"

namespace snot {
	VALUE Object::send(const char* message, uint64_t num_args, ...) {
		va_list ap;
		va_start(ap, num_args);
		VALUE ret = va_send(message, num_args, ap);
		va_end(ap);
		return ret;
	}
	
	VALUE Object::va_send(const char* message, uint64_t num_args, va_list& ap) {
		printf("LOL calling %s (%d args)\n", message, num_args);
	}
}
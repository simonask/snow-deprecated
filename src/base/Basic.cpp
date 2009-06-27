#include "Basic.h"
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

namespace snow {
	void warn(const char* msg, ...) {
		va_list ap;
		va_start(ap, msg);
		char* interp;
		vasprintf(&interp, msg, ap);
		fprintf(stderr, "WARNING: %s\n", interp);
		free(interp);
		va_end(ap);
	}
	
	void error(const char* msg, ...) {
		va_list ap;
		va_start(ap, msg);
		char* interp;
		vasprintf(&interp, msg, ap);
		fprintf(stderr, "ERROR: %s\n", interp);
		free(interp);
		va_end(ap);
	}
	
	#ifdef DEBUG
	void debug(const char* msg, ...) {
		va_list ap;
		va_start(ap, msg);
		char* interp;
		vasprintf(&interp, msg, ap);
		fprintf(stderr, "DEBUG: %s\n", interp);
		free(interp);
		va_end(ap);
	}
	#endif
}

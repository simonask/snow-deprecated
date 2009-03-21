#ifndef GLOBAL_H_OR4WZ1AZ
#define GLOBAL_H_OR4WZ1AZ

#include "Value.h"

namespace snow {
	class Scope;
	
	class Global {
	private:
		Global() {}
		
		static VALUE puts(VALUE self, uint64_t num_args, VALUE* args);
		static VALUE print(VALUE self, uint64_t num_args, VALUE* args);
		static VALUE throw_exception(VALUE self, uint64_t num_args, VALUE* args);
	public:
		static void define_globals(Scope& scope);
	};
}

#endif /* end of include guard: GLOBAL_H_OR4WZ1AZ */

#include "Global.h"
#include "Runtime.h"
#include "Scope.h"

#include "Object.h"
#include "Integer.h"
#include "Float.h"
#include "Nil.h"
#include "Array.h"

// Convenience
#define FUNC(name) (new(kMalloc) Function(name))

namespace snow {
	void Global::define_globals(Scope& scope) {
		scope.set_local("Object", object_prototype());
		scope.set_local("Integer", integer_prototype());
		scope.set_local("Float", float_prototype());
		scope.set_local("Nil", nil_prototype());
		scope.set_local("Array", array_prototype());
		scope.set_local("@", array_prototype());
		
		// Base functions
		scope.set_local("puts", FUNC(Global::puts));
		scope.set_local("print", FUNC(Global::print));
		scope.set_local("throw", FUNC(Global::throw_exception));
	}
	
	VALUE Global::puts(VALUE self, uint64_t num_args, VALUE* args) {
		for (uint64_t i = 0; i < num_args; ++i) {
			printf("%s", value_to_string(args[i]));
		}
		printf("\n");
		return nil();
	}
	
	VALUE Global::print(VALUE self, uint64_t num_args, VALUE* args) {
		for (uint64_t i = 0; i < num_args; ++i) {
			printf("%s", value_to_string(args[i]));
		}
		return nil();
	}
	
	VALUE Global::throw_exception(VALUE self, uint64_t num_args, VALUE* args) {
		error("Exceptions aren't implemented yet, trapping...");
		TRAP();
	}
}
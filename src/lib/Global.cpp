#include "Global.h"
#include "Runtime.h"
#include "Scope.h"

#include "Object.h"
#include "Integer.h"
#include "Float.h"
#include "Nil.h"
#include "Array.h"
#include "Garbage.h"

// Convenience
#define FUNC(name) (new(kMalloc) Function(name))

namespace snow {
	static VALUE puts(VALUE self, uint64_t num_args, VALUE* args) {
		for (uint64_t i = 0; i < num_args; ++i) {
			printf("%s", value_to_string(args[i]));
		}
		printf("\n");
		return nil();
	}
	
	static VALUE print(VALUE self, uint64_t num_args, VALUE* args) {
		for (uint64_t i = 0; i < num_args; ++i) {
			printf("%s", value_to_string(args[i]));
		}
		return nil();
	}
	
	static VALUE throw_exception(VALUE self, uint64_t num_args, VALUE* args) {
		error("Exceptions aren't implemented yet, trapping...");
		TRAP();
	}
	
	static VALUE collect_garbage(VALUE, uint64_t, VALUE*) {
		Garbage::collect();
		return nil();
	}
	
	void Global::define_globals(Scope& scope) {
		scope.set_local("Object", object_prototype());
		scope.set_local("Integer", integer_prototype());
		scope.set_local("Float", float_prototype());
		scope.set_local("Nil", nil_prototype());
		scope.set_local("Array", array_prototype());
		scope.set_local("@", array_prototype());
		
		// Base functions
		scope.set_local("puts", FUNC(puts));
		scope.set_local("print", FUNC(print));
		scope.set_local("throw", FUNC(throw_exception));
		scope.set_local("collect_garbage", FUNC(collect_garbage));
	}
}
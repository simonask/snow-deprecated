#include "Global.h"
#include "Runtime.h"
#include "Scope.h"

#include "Object.h"
#include "Integer.h"
#include "Float.h"
#include "Nil.h"
#include "Array.h"
#include "Garbage.h"
#include "SnowString.h"

// Convenience
#define FUNC(name) (new(kMalloc) Function(name))

#include <sstream>
using namespace std;

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

	static VALUE garbage_stats(VALUE, uint64_t, VALUE*) {
		std::stringstream ss;
		const IAllocator::Statistics& stats = MemoryManager::statistics(kGarbage);
		ss << "allocated objects: " << stats.allocated_objects << endl;
		ss << "allocated size: " << stats.allocated_size << endl;
		ss << "freed objects: " << stats.freed_objects << endl;
		ss << "freed size: " << stats.freed_size << endl;
		return new String(ss.str());
	}
	
	void Global::define_globals(Scope& scope) {
		scope.set_local("Object", object_prototype());
		scope.set_local("Integer", integer_prototype());
		scope.set_local("Float", float_prototype());
		scope.set_local("Nil", nil_prototype());
		scope.set_local("Array", array_prototype());
		scope.set_local("String", string_prototype());
		scope.set_local("@", array_prototype());
		
		// Base functions
		scope.set_local("puts", FUNC(puts));
		scope.set_local("print", FUNC(print));
		scope.set_local("throw", FUNC(throw_exception));
		scope.set_local("collect_garbage", FUNC(collect_garbage));
		scope.set_local("garbage_stats", FUNC(garbage_stats));
	}
}

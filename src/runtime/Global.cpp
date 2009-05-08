#include "Global.h"
#include "Kernel.h"
#include "Runtime.h"
#include "Scope.h"

#include "Object.h"
#include "Integer.h"
#include "Float.h"
#include "Nil.h"
#include "Array.h"
#include "Hash.h"
#include "SnowString.h"
#include "Exception.h"

#include "gc/Garbage.h"

// Convenience
#define FUNC(name) (new(kMalloc) Function(name))

#include <sstream>
using namespace std;

namespace snow {
	static VALUE require(VALUE, uint64_t num_args, VALUE* args) {
		for (uint64_t i = 0; i < num_args; ++i) {
			const char* str = value_to_string(args[i]);
			Kernel::require(str);
		}
		return nil();
	}

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
	

	static VALUE current_scope(VALUE self, uint64_t num_args, VALUE* args) {
		StackFrame* frame = get_current_stack_frame();
		if (!frame) {
			return nil();
		}
		return frame->scope;
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

	static VALUE try_closure(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		if (num_args < 1) return nil();
		ExceptionHandler handler;

		Handle<Object> result = new Object;

		if (TRY_CATCH(handler)) {
			result->set_raw_s("value", snow::call(self, args[0], 0));
		} else {
			result->set_raw_s("exception", handler.exception());
		}
		return result;
	}
	
	static VALUE throw_exception_internal(VALUE self, uint64_t num_args, VALUE* args) {
		throw_exception(num_args > 0 ? args[0] : nil());
		return nil();
	}

	void Global::define_globals(Scope& scope) {
		scope.set_local_by_string("Object", object_prototype());
		scope.set_local_by_string("Integer", integer_prototype());
		scope.set_local_by_string("Float", float_prototype());
		scope.set_local_by_string("Nil", nil_prototype());
		scope.set_local_by_string("Array", array_prototype());
		scope.set_local_by_string("Hash", hash_prototype());
		scope.set_local_by_string("String", string_prototype());
		scope.set_local_by_string("@", array_prototype()->get_raw_s("new"));
		
		// Base functions
		scope.set_local_by_string("require", FUNC(require));
		scope.set_local_by_string("puts", FUNC(puts));
		scope.set_local_by_string("print", FUNC(print));
		scope.set_local_by_string("current_scope", FUNC(current_scope));
		scope.set_local_by_string("collect_garbage", FUNC(collect_garbage));
		scope.set_local_by_string("garbage_stats", FUNC(garbage_stats));
		scope.set_local_by_string("try", FUNC(try_closure));
		scope.set_local_by_string("throw", FUNC(throw_exception_internal));
	}
}

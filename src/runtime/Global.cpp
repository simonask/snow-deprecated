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
#include "ExternalLibrary.h"

#include "gc/Garbage.h"

#include <sstream>
using namespace std;

#include <dlfcn.h>

namespace snow {
	static Value require(const Value&, const Arguments& args) {
		for (uintx i = 0; i < args.size; ++i) {
			const char* str = value_to_string(args.data[i]);
			Kernel::require(str);
		}
		return nil();
	}

	static Value puts(const Value& self, const Arguments& args) {
		for (uintx i = 0; i < args.size; ++i) {
			printf("%s", value_to_string(args.data[i]));
		}
		printf("\n");
		return nil();
	}
	
	static Value print(const Value& self, const Arguments& args) {
		for (uintx i = 0; i < args.size; ++i) {
			printf("%s", value_to_string(args.data[i]));
		}
		return nil();
	}
	

	static Value current_scope(const Value& self, const Arguments& args) {
		StackFrame* frame = get_current_stack_frame();
		if (!frame) {
			return nil();
		}
		return frame->scope;
	}
	
	static Value collect_garbage(const Value& self, const Arguments& args) {
		Garbage::collect();
		return nil();
	}

	static Value garbage_stats(const Value& self, const Arguments& args) {
		std::stringstream ss;
		const IAllocator::Statistics& stats = MemoryManager::statistics(kGarbage);
		ss << "allocated objects: " << stats.allocated_objects << endl;
		ss << "allocated size: " << stats.allocated_size << endl;
		ss << "freed objects: " << stats.freed_objects << endl;
		ss << "freed size: " << stats.freed_size << endl;
		return gc_new<String>(ss.str());
	}

	static Value try_closure(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		if (args.size < 1) return nil();
		ExceptionHandler handler;

		Handle<Object> result = gc_new<Object>();

		if (TRY_CATCH(handler)) {
			result->set_raw("value", snow::call(self, args.data[0]));
		} else {
			result->set_raw("exception", handler.exception());
		}
		return result;
	}
	
	static Value throw_exception_internal(const Value& self, const Arguments& args) {
		throw_exception(args.size > 0 ? args.data[0] : nil());
		return nil();
	}

	static Value internal_dlopen(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		if (args.size < 1)
			throw_exception(gc_new<String>("Expected 1 argument for dlopen()."));
		Handle<String> lib_name = object_cast<String>(args.data[0]);
		if (!lib_name && args.data[0] != nil())
			throw_exception(gc_new<String>("Expected the name of a library or nil."));

		const char* str = NULL;
		if (lib_name)
			str = lib_name->c_str();

		void* handle = dlopen(str, RTLD_LAZY | RTLD_LOCAL);

		return gc_new<ExternalLibrary>(handle);
	}

	void Global::define_globals(const Ptr<Scope>& scope) {
		scope->set_local("Object", object_prototype());
		scope->set_local("Integer", integer_prototype());
		scope->set_local("Float", float_prototype());
		scope->set_local("Nil", nil_prototype());
		scope->set_local("Array", array_prototype());
		scope->set_local("Hash", hash_prototype());
		scope->set_local("String", string_prototype());
		scope->set_local("ExternalLibrary", external_library_prototype());
		scope->set_local("@", array_prototype()->get_raw("new"));
		
		// Base functions
		scope->set_local("require", gc_new<Function>(require));
		scope->set_local("puts", gc_new<Function>(puts));
		scope->set_local("print", gc_new<Function>(print));
		scope->set_local("current_scope", gc_new<Function>(current_scope));
		scope->set_local("collect_garbage", gc_new<Function>(collect_garbage));
		scope->set_local("garbage_stats", gc_new<Function>(garbage_stats));
		scope->set_local("try", gc_new<Function>(try_closure));
		scope->set_local("throw", gc_new<Function>(throw_exception_internal));
		scope->set_local("dlopen", gc_new<Function>(internal_dlopen));
	}
}

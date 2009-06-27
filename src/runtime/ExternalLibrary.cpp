#include "ExternalLibrary.h"
#include "Function.h"
#include "codegen/Codegen.h"
#include "runtime/SnowString.h"
#include "runtime/Exception.h"
#include "runtime/Kernel.h"

#include <dlfcn.h>

namespace snow {
	Ptr<Function> ExternalLibrary::compile_proxy(const char* symbol_name, const ExternalLibrary::FunctionSignature& sig) {
		void* function_pointer = dlsym(m_Handle, symbol_name);
		Ptr<CompiledCode> code = Codegen::compile_proxy(function_pointer, sig);
		code->link(Kernel::linker_symbols());
		code->make_executable();
		return gc_new<Function>((NativeFunctionPtr)code->code());
	}

	ExternalLibrary::~ExternalLibrary() {
		if (m_Handle)
			dlclose(m_Handle);
	}

	static ExternalLibrary::NativeType symbol_to_native_type(Symbol sym) {
		if (!is_symbol(sym))
			throw_exception(gc_new<String>("Expected symbol argument."));

		static const Symbol sym_void("void");
		static const Symbol sym_pointer("pointer");
		static const Symbol sym_int("int");
		static const Symbol sym_float("float");
		static const Symbol sym_string("string");
		static const Symbol sym_value("value");

		if (sym == sym_void)
			return ExternalLibrary::NATIVE_VOID;
		else if (sym == sym_pointer)
			return ExternalLibrary::NATIVE_POINTER;
		else if (sym == sym_int)
			return ExternalLibrary::NATIVE_INT;
		else if (sym == sym_float)
			return ExternalLibrary::NATIVE_FLOAT;
		else if (sym == sym_string)
			return ExternalLibrary::NATIVE_STRING;
		else if (sym == sym_value)
			return ExternalLibrary::NATIVE_VALUE;
		else
			throw_exception(gc_new<String>("Unknown native type: %", (VALUE)sym));

		return ExternalLibrary::NATIVE_VOID; // dummy
	}

	static Value external_library_function(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		Handle<ExternalLibrary> lib = object_cast<ExternalLibrary>(self);
		ASSERT(lib);
		if (args.size < 1)
			throw_exception(gc_new<String>("Expected at least 1 arguments for ExternalLibrary.function()."));
		const char* sym = value_to_string(args.data[0]);

		// Create signature from arguments
		uintx proxy_args = args.size < 2 ? 0 : args.size - 2;
		ExternalLibrary::FunctionSignature signature(proxy_args);
		signature.return_type = args.size < 2 ? ExternalLibrary::NATIVE_VOID : symbol_to_native_type(args.data[1]);
		for (uintx i = 0; i < proxy_args; ++i) {
			signature.arg_types[i] = symbol_to_native_type(args.data[i+2]);
		}

		return lib->compile_proxy(sym, signature);
	}

	Ptr<Object> external_library_prototype() {
		static Ptr<Object> proto;
		if (proto) return proto;
		proto = gc_new<Object>();
		proto->set_raw("function", gc_new<Function>(external_library_function));
		return proto;
	}

	void external_library_function_num_args_mismatch(uintx expected, uintx given) {
		ASSERT(expected != given);
		throw_exception(gc_new<String>("Native function with wrong number of arguments; expected %, got %.", value((intx)expected), value((intx)given)));
	}

	void* convert_value_to_native(VALUE val, ExternalLibrary::NativeType type) {
		switch (type) {
			case ExternalLibrary::NATIVE_VOID:
				return NULL;
			case ExternalLibrary::NATIVE_POINTER:
				// TODO: Pointer containers.
			case ExternalLibrary::NATIVE_VALUE:
				return val;
			case ExternalLibrary::NATIVE_INT:
				if (!is_integer(val))
					val = snow::call_method(val, "to_integer").value();
				ASSERT(is_integer(val));
				return reinterpret_cast<void*>(integer(val));
			case ExternalLibrary::NATIVE_FLOAT:
			{
				if (!is_float(val))
					val = snow::call_method(val, "to_float").value();
				ASSERT(is_float(val));
				float f = floatnum(val);
				void* p = NULL;
				memcpy(&p, &f, sizeof(float));
				return p;
			}
			case ExternalLibrary::NATIVE_STRING:
			{
				const char* cstr = value_to_string(val);
				return reinterpret_cast<void*>(const_cast<char*>(cstr));
			}
			default:
				ASSERT(false && "Unknown native type?");
				return NULL;
		}
	}

	VALUE convert_native_to_value(void* native, ExternalLibrary::NativeType type) {
		switch (type) {
			case ExternalLibrary::NATIVE_VOID:
				return nil();
			case ExternalLibrary::NATIVE_POINTER:
				// TODO: Pointer containers.
				return nil();
			case ExternalLibrary::NATIVE_VALUE:
				return native;
			case ExternalLibrary::NATIVE_INT:
				return value(reinterpret_cast<intx>(native));
			case ExternalLibrary::NATIVE_FLOAT:
				float f;
				memcpy(&f, &native, sizeof(float));
				return value(f);
			case ExternalLibrary::NATIVE_STRING:
			{
				const char* cstr = reinterpret_cast<const char*>(native);
				return gc_new<String>(cstr).value();
			}
			default:
				ASSERT(false && "Unknown native type?");
				return NULL;
		}
	}


}

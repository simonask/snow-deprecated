#include "Kernel.h"
#include "runtime/Runtime.h"
#include "runtime/Global.h"
#include "runtime/Function.h"
#include "runtime/SnowString.h"
#include "runtime/Exception.h"
#include "runtime/ExternalLibrary.h"
#include "runtime/Arguments.h"
#include "codegen/ASTNode.h"
#include "parser/Driver.h"
#include "gc/Garbage.h"

#include <fstream>

namespace snow {
	
	void Kernel::init() {
		static bool initted = false;
		if (initted)
			return;
		initted = true;

		Garbage::init_fence();

		// Preload prototypes
		string_prototype_init();
		function_prototype_init();
		scope_prototype_init();
		array_prototype_init();
		
		// Set up runtime symbols needed for linking and compiling snow sources.
		Linker::SymbolTable& table = linker_symbols();
		table["snow_eval_truth"] = (void*)snow::eval_truth;
		table["snow_call"] = (void*)snow::call_internal;
		table["snow_get"] = (void*)snow::get_internal;
		table["snow_set"] = (void*)snow::set_internal;
		table["snow_enter_scope"] = (void*)snow::enter_scope;
		table["snow_leave_scope"] = (void*)snow::leave_scope;
		table["snow_set_parent_scope"] = (void*)snow::set_parent_scope;
		table["snow_get_local"] = (void*)snow::get_local_internal;
		table["snow_set_local"] = (void*)snow::set_local_internal;

		table["snow_external_library_function_num_args_mismatch"] = (void*)snow::external_library_function_num_args_mismatch;
		table["snow_convert_native_to_value"] = (void*)snow::convert_native_to_value;
		table["snow_convert_value_to_native"] = (void*)snow::convert_value_to_native;
		
		// Bind basic standard library stuff in global scope.
		Global::define_globals(global_scope());
		
	}
	
	Value Kernel::require(const std::string& file) {
		init();
		HandleScope _s;
		
		std::ifstream f(file.c_str());
		RefPtr<ast::FunctionDefinition> scope = Driver::parse(f, file);
		f.close();
		scope->name = "<global>";
		
		if (!scope)
			throw_exception(gc_new<String>("An error occurred while parsing file `%'.", gc_new<String>(file.c_str())));
		
		RefPtr<Codegen> codegen = Codegen::create(*scope);
		Handle<CompiledCode> cc = codegen->compile(true);
		
		cc->link(linker_symbols());
		
		// TODO: Delay make_executable?
		cc->make_executable();
		
		Handle<Function> func = gc_new<Function>(*cc);
		
		return func->call_in_scope(global_scope());
	}
	
	Value Kernel::eval(const std::string& str) {
		init();
		HandleScope _s;
		
		RefPtr<ast::FunctionDefinition> scope = Driver::parse(str, "<eval>");
		
		if (!scope)
			throw_exception(gc_new<String>("An error occurred while parsing `%'.", gc_new<String>(str.c_str())));

		scope->name = "<eval>";
		
		RefPtr<Codegen> codegen = Codegen::create(*scope);
		Handle<CompiledCode> cc = codegen->compile();
		
		cc->link(linker_symbols());
		
		// TODO: Delay make_executable?
		cc->make_executable();
		
		Handle<Function> func = gc_new<Function>(*cc);
		
		Ptr<Scope> parent_scope;
		StackFrame* current_stack_frame = get_current_stack_frame();
		if (current_stack_frame)
			parent_scope = current_stack_frame->scope;
		else
			parent_scope = global_scope();

		func->set_parent_scope(parent_scope);
		return func->call(nil(), Arguments());
	}
	
	Value Kernel::eval_in_global_scope(const std::string& input) {
		init();
		HandleScope _s;
		
		RefPtr<ast::FunctionDefinition> scope = Driver::parse(input, "<eval>");
		
		if (!scope)
			throw_exception(gc_new<String>("An error occurred while parsing `%'.", gc_new<String>(input.c_str())));

		scope->name = "<global>";
		
		RefPtr<Codegen> codegen = Codegen::create(*scope);
		Handle<CompiledCode> cc = codegen->compile(true);
		
		cc->link(linker_symbols());
		
		// TODO: Delay make_executable?
		cc->make_executable();
		
		Handle<Function> func = gc_new<Function>(*cc);
		
		return func->call_in_scope(global_scope());
	}

	Linker::SymbolTable& Kernel::linker_symbols() {
		static Linker::SymbolTable s;
		return s;
	}
	
	Ptr<Scope> Kernel::global_scope() {
		static Ptr<Scope> s = gc_new<Scope>();
		return s;
	}
}

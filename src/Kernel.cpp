#include "Kernel.h"
#include "lib/Runtime.h"
#include "lib/Global.h"
#include "lib/Function.h"
#include "lib/SnowString.h"
#include "lib/Exception.h"
#include "ASTNode.h"
#include "parser/Driver.h"

#include "x86_64/x86_64-Disassembler.h"

#include <fstream>

namespace snow {
	
	void Kernel::init() {
		static bool initted = false;
		if (initted)
			return;
		initted = true;
		
		// Set up runtime symbols needed for linking and compiling snow sources.
		Linker::SymbolTable& table = linker_symbols();
		table["snow_eval_truth"] = (void*)snow::eval_truth;
		table["snow_call"] = (void*)snow::call;
		table["snow_get"] = (void*)snow::get;
		table["snow_set"] = (void*)snow::set;
		table["snow_enter_scope"] = (void*)snow::enter_scope;
		table["snow_leave_scope"] = (void*)snow::leave_scope;
		table["snow_set_parent_scope"] = (void*)snow::set_parent_scope;
		table["snow_get_local"] = (void*)snow::get_local;
		table["snow_set_local"] = (void*)snow::set_local;
		
		// Bind basic standard library stuff in global scope.
		Global::define_globals(global_scope());
		
	}
	
	VALUE Kernel::require(const std::string& file) {
		init();
		HandleScope _s;
		
		std::ifstream f(file.c_str());
		RefPtr<ast::FunctionDefinition> scope = Driver::parse(f, file);
		f.close();
		
		if (!scope)
			throw_exception(new String("An error occurred while parsing file `%'.", new String(file.c_str())));
		
		RefPtr<Codegen> codegen = Codegen::create(*scope);
		Handle<CompiledCode> cc = codegen->compile(true);
		
		cc->link(linker_symbols());
		
		// TODO: Delay make_executable?
		cc->make_executable();
		
		Handle<Function> func = new Function(*cc);
		
		return func->call_in_scope(&global_scope());
	}
	
	VALUE Kernel::eval(const std::string& str) {
		init();
		HandleScope _s;
		
		RefPtr<ast::FunctionDefinition> scope = Driver::parse(str, "<eval>");
		
		if (!scope)
			throw_exception(new String("An error occurred while parsing `%'.", new String(str.c_str())));
		
		RefPtr<Codegen> codegen = Codegen::create(*scope);
		Handle<CompiledCode> cc = codegen->compile();
		
		cc->link(linker_symbols());
		
		// TODO: Delay make_executable?
		cc->make_executable();
		
		Handle<Function> func = new Function(*cc);
		
		Scope* parent_scope;
		StackFrame* current_stack_frame = get_current_stack_frame();
		if (current_stack_frame)
			parent_scope = current_stack_frame->scope;
		else
			parent_scope = &global_scope();

		func->set_parent_scope(parent_scope);
		return func->call(nil(), 0);
	}
	
	VALUE Kernel::eval_in_global_scope(const std::string& input) {
		init();
		HandleScope _s;
		
		RefPtr<ast::FunctionDefinition> scope = Driver::parse(input, "<eval>");
		
		if (!scope)
			throw_exception(new String("An error occurred while parsing `%'.", new String(input.c_str())));
		
		RefPtr<Codegen> codegen = Codegen::create(*scope);
		Handle<CompiledCode> cc = codegen->compile(true);
		
		cc->link(linker_symbols());
		
		// TODO: Delay make_executable?
		cc->make_executable();
		
		Handle<Function> func = new Function(*cc);
		
		return func->call_in_scope(&global_scope());
	}

	Linker::SymbolTable& Kernel::linker_symbols() {
		static Linker::SymbolTable s;
		return s;
	}
	
	Scope& Kernel::global_scope() {
		static Scope* s = new(kMalloc) Scope;
		return *s;
	}
}

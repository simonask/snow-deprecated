#include "Kernel.h"
#include "lib/Runtime.h"
#include "lib/Global.h"
#include "lib/Function.h"
#include "ASTNode.h"
#include "parser/Driver.h"

#include "x86_64/Disassembler.h"

#include <fstream>

namespace snow {
	void Kernel::init() {
		static bool initted = false;
		if (initted)
			return;
		
		// Set up runtime symbols needed for linking and compiling snow sources.
		SymbolTable& table = runtime_symbols();
		table["snow_eval_truth"] = (void*)snow::eval_truth;
		table["snow_call"] = (void*)snow::call;
		table["snow_get"] = (void*)snow::get;
		table["snow_set"] = (void*)snow::set;
		table["snow_enter_scope"] = (void*)snow::enter_scope;
		table["snow_leave_scope"] = (void*)snow::leave_scope;
		table["snow_set_parent_scope"] = (void*)snow::set_parent_scope;
		table["snow_get_local"] = (void*)snow::get_local;
		
		// Bind basic standard library stuff in global scope.
		Global::define_globals(global_scope());
		
		initted = true;
	}
	
	VALUE Kernel::require(const std::string& file) {
		init();
		
		std::ifstream f(file.c_str());
		RefPtr<ast::FunctionDefinition> scope = Driver::parse(f, file);
		f.close();
		
		if (!scope) {
			error("An unknown error occured while parsing `%s'", file.c_str());
			TRAP();
		}
		
		RefPtr<Codegen> codegen = Codegen::create(*scope);
		RefPtr<CompiledCode> cc = codegen->compile();
		
		cc->link(runtime_symbols());
		
		#ifdef DEBUG
		std::ofstream disasmfile(std::string(file + ".s").c_str());
		disasmfile << x86_64::Disassembler::disassemble(*cc, runtime_symbols());
		disasmfile.close();
		#endif
		
		// TODO: Delay make_executable?
		cc->make_executable();
		
		Handle<Function> func = new Function(*cc);
		
		// TODO: We can't call with global scope because of the way locals work... Yet!
		//return func->call_in_scope(&global_scope());
		func->set_parent_scope(&global_scope());
		return func->call(nil(), 0);
	}
	
	VALUE Kernel::eval(const std::string& str) {
		init();
		
		RefPtr<ast::FunctionDefinition> scope = Driver::parse(str, "<eval>");
		
		if (!scope) {
			error("An unknown error occured while parsing `%s'", "<eval>");
			TRAP();
		}
		
		RefPtr<Codegen> codegen = Codegen::create(*scope);
		RefPtr<CompiledCode> cc = codegen->compile();
		
		cc->link(runtime_symbols());
		
		// TODO: Delay make_executable?
		cc->make_executable();
		
		Handle<Function> func = new Function(*cc);
		
		// TODO: We can't call with global scope because of the way locals work... Yet!
		//return func->call_in_scope(&global_scope());
		func->set_parent_scope(&global_scope());
		return func->call(nil(), 0);
	}
	
	SymbolTable& Kernel::runtime_symbols() {
		static SymbolTable s;
		return s;
	}
	
	Scope& Kernel::global_scope() {
		static Scope s;
		return s;
	}
}
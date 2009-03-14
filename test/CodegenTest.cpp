#include "test.h"
#include "lib/Function.h"
#include "Codegen.h"
#include "ASTNode.h"
#include "lib/Runtime.h"
#include "x86_64/Disassembler.h"
using namespace snow;
using namespace ast;

#include <iostream>

TEST_SUITE(Codegen);

// convenience for building ASTs
#define _function new FunctionDefinition
#define _assign new Assignment
#define _ident new Identifier
#define _call new Call
#define _seq new Sequence
#define _lit_int(val) new Literal(#val, Literal::INTEGER_TYPE)

static SymbolTable table = SymbolTable();

static Handle<Function> compile(RefPtr<FunctionDefinition> def) {
	RefPtr<Codegen> codegen = Codegen::create(*def);
	
	table["snow_eval_truth"] = (void*)snow::eval_truth;
	table["snow_call"] = (void*)snow::call;
	table["snow_get"] = (void*)snow::get;
	table["snow_set"] = (void*)snow::set;
	table["snow_enter_scope"] = (void*)snow::enter_scope;
	table["snow_leave_scope"] = (void*)snow::leave_scope;
	table["snow_get_local"] = (void*)snow::get_local;
	table["snow_set_parent_scope"] = (void*)snow::set_parent_scope;
	
	RefPtr<CompiledCode> cc = codegen->compile();
	cc->export_symbols(table);
	cc->link(table);
	cc->make_executable();
	
	return new Function(cc);
}

static void dump_disasm(std::ostream& os, Handle<Function> func) {
	os << std::endl;
	os << x86_64::Disassembler::disassemble(*func->code(), table);
}

TEST_CASE(simple_add) {
	RefPtr<FunctionDefinition> def = _function(
		_assign(_ident("a"), _lit_int(123)),
		_assign(_ident("b"), _lit_int(456)),
		_call(_ident("a"), _ident("+"), _seq(_ident("b")))
	);
	
	Handle<Function> f = compile(def);
	ValueHandle ret = snow::call(NULL, f, 0);
	
	TEST_EQ(integer(ret), 579);
}

TEST_CASE(simple_closure) {
	RefPtr<FunctionDefinition> def = _function(
		_assign(_ident("a"), _lit_int(123)),
		_assign(_ident("func"),
			_function(
				_assign(_ident("b"), _lit_int(456)),
				_call(_ident("b"), _ident("+"), _seq(_ident("a")))
			)
		),
		_call(_ident("func"))
	);
	
	Handle<Function> f = compile(def);
	//dump_disasm(std::cout, f);
	ValueHandle ret = snow::call(NULL, f, 0);
	
	TEST_EQ(integer(ret), 579);
}

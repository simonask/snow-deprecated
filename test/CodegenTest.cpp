#include "test.h"
#include "lib/Function.h"
#include "Codegen.h"
#include "ASTNode.h"
#include "lib/Runtime.h"
#include "x86_64/x86_64-Disassembler.h"
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
#define _lit_str(val) new Literal(val, Literal::STRING_TYPE)
#define _get new Get
#define _set new Set
#define _loop new Loop
#define _return new Return
#define _if new IfCondition
#define _if_else new IfElseCondition

static Linker::SymbolTable table = Linker::SymbolTable();


static void dump_disasm(std::ostream& os, const CompiledCode& code) {
	os << std::endl;
	os << x86_64::Disassembler::disassemble(code, table);
}

static Handle<Function> compile(RefPtr<FunctionDefinition> def) {
	RefPtr<Codegen> codegen = Codegen::create(*def);
	
	table["snow_eval_truth"] = (void*)snow::eval_truth;
	table["snow_call"] = (void*)snow::call;
	table["snow_get"] = (void*)snow::get;
	table["snow_set"] = (void*)snow::set;
	table["snow_enter_scope"] = (void*)snow::enter_scope;
	table["snow_leave_scope"] = (void*)snow::leave_scope;
	table["snow_get_local"] = (void*)snow::get_local;
	table["snow_set_local"] = (void*)snow::set_local;
	table["snow_set_parent_scope"] = (void*)snow::set_parent_scope;
	
	Handle<CompiledCode> cc = codegen->compile(false);
	cc->export_symbols(table);
	cc->link(table);
	cc->make_executable();
	
	//dump_disasm(std::cout, *cc);
	
	return new Function(*cc);
}

TEST_CASE(simple_add) {
	HandleScope _;
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
	HandleScope _;

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

TEST_CASE(object_get) {
	HandleScope _;
	RefPtr<FunctionDefinition> def = _function(
		_get(_ident("obj"), _ident("member"))
	);
	def->arguments.push_back(_ident("obj"));
	
	Handle<Object> obj = new Object;
	obj->set_raw_s("member", value(456LL));
	
	Handle<Function> f = compile(def);
	ValueHandle ret = snow::call(NULL, f, 1, obj.value());
	
	TEST_EQ(integer(ret), 456LL);
}

TEST_CASE(object_set) {
	HandleScope _;
	RefPtr<FunctionDefinition> def = _function(
		_set(_ident("obj"), _ident("member"), _ident("value"))
	);
	def->arguments.push_back(_ident("obj"));
	def->arguments.push_back(_ident("value"));
	
	Handle<Object> obj = new Object;
	Handle<Function> f = compile(def);
	ValueHandle ret = snow::call(NULL, f, 2, obj.value(), value(456LL));
	
	TEST_EQ(integer(obj->get_raw_s("member")), 456LL);
}

TEST_CASE(simple_loop) {
	HandleScope _;
	RefPtr<FunctionDefinition> def = _function(
		_assign(_ident("a"), _lit_int(0)),
		_loop(
			// condition:
			_call(_ident("a"), _ident("<"), _seq(_lit_int(1000))),
			// body:
			_assign(_ident("a"), _call(_ident("a"), _ident("+"), _seq(_lit_int(1))))
		),
		_ident("a")
	);
	
	Handle<Function> f = compile(def);
	ValueHandle ret = snow::call(NULL, f, 0);
	
	TEST_EQ(integer(ret), 1000LL);
}

TEST_CASE(premature_return) {
	HandleScope _;
	RefPtr<FunctionDefinition> def = _function(
		_return(_lit_int(123)),
		_lit_int(456)
	);
	
	Handle<Function> f = compile(def);
	ValueHandle ret = snow::call(NULL, f, 0);
	TEST_EQ(integer(ret), 123LL);
}

TEST_CASE(if_condition) {
	HandleScope _;
	RefPtr<FunctionDefinition> def = _function(
		_if(
			// condition:
			_call(_ident("a"), _ident("="), _seq(_lit_str("This is a test"))),
			// body:
			_return(_lit_int(1))
		),
		_lit_int(0)
	);
	def->arguments.push_back(_ident("a"));
	
	Handle<Function> f = compile(def);
	TEST_EQ(snow::call(NULL, f, 0), value(0LL));
	TEST_EQ(snow::call(NULL, f, 1, create_string("This is a test")), value(1LL));
	TEST_EQ(snow::call(NULL, f, 1, value(123LL)), value(0LL));
}

TEST_CASE(unless_condition) {
	HandleScope _;
	RefPtr<FunctionDefinition> def = _function(
		_if(
			// condition:
			_call(_ident("a"), _ident("="), _seq(_lit_str("A test, this is"))),
			// body:
			_return(_lit_int(1)),
			// unless?:
			true
		),
		_lit_int(0)
	);
	def->arguments.push_back(_ident("a"));
	
	Handle<Function> f = compile(def);
	TEST_EQ(snow::call(NULL, f, 0), value(1LL));
	TEST_EQ(snow::call(NULL, f, 1, create_string("A test, this is")), value(0LL));
	TEST_EQ(snow::call(NULL, f, 1, value(123LL)), value(1LL));
}

TEST_CASE(if_else_condition) {
	HandleScope _;
	// also tests the result of if-expressions
	RefPtr<FunctionDefinition> def = _function(
		_if_else(
			// condition:
			_call(_ident("a"), _ident("="), _seq(_lit_str("This is a test"))),
			// if true:
			_lit_int(123LL),
			// if false:
			_lit_int(456LL)
		)
	);
	def->arguments.push_back(_ident("a"));
	
	Handle<Function> f = compile(def);
	TEST_EQ(snow::call(NULL, f, 0), value(456LL));
	TEST_EQ(snow::call(NULL, f, 1, create_string("This is a test")), value(123LL));
	TEST_EQ(snow::call(NULL, f, 1, create_string("LOL")), value(456LL));
}

TEST_CASE(unless_else_condition) {
	HandleScope _;
	// also tests the result of unless-expressions
	RefPtr<FunctionDefinition> def = _function(
		_if_else(
			// condition:
			_call(_ident("a"), _ident("="), _seq(_lit_str("A test, this is"))),
			// if true:
			_lit_int(999LL),
			// if false:
			_lit_int(771LL),
			// unless?:
			true
		)
	);
	def->arguments.push_back(_ident("a"));
	
	Handle<Function> f = compile(def);
	TEST_EQ(snow::call(NULL, f, 0), value(999LL));
	TEST_EQ(snow::call(NULL, f, 1, create_string("A test, this is")), value(771LL));
	TEST_EQ(snow::call(NULL, f, 1, create_string("F00B4RZ!")), value(999LL));
}

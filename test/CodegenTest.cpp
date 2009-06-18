#include "test.h"
#include "runtime/Function.h"
#include "codegen/Codegen.h"
#include "codegen/ASTNode.h"
#include "runtime/Runtime.h"
#include "runtime/SnowString.h"
#include "codegen/x86_64/x86_64-Disassembler.h"
using namespace snow;
using namespace ast;

#include <iostream>

TEST_SUITE(Codegen);

// convenience for building ASTs
#define _function new FunctionDefinition
#define _local_assign new LocalAssignment
#define _member_assign new MemberAssignment
#define _ident new Identifier
#define _member_call new MemberCall
#define _local_call new ExpressionCall
#define _seq new Sequence
#define _lit_int(val) new Literal(#val, Literal::INTEGER_TYPE)
#define _lit_bool(val) new Literal(val ? Literal::TRUE_TYPE : Literal::FALSE_TYPE)
#define _lit_str(val) new Literal(val, Literal::STRING_TYPE)
#define _member new Member
#define _loop new Loop
#define _return new Return
#define _if new IfCondition
#define _if_else new IfElseCondition
#define _logical_and new LogicalAnd
#define _logical_or new LogicalOr
#define _logical_xor new LogicalXor
#define _logical_not new LogicalNot

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
	
	return gc_new<Function>(*cc);
}

TEST_CASE(simple_add) {
	HandleScope _;
	RefPtr<FunctionDefinition> def = _function(
		_local_assign(_ident("a"), _lit_int(123)),
		_local_assign(_ident("b"), _lit_int(456)),
		_member_call(_ident("a"), _ident("+"), _seq(_ident("b")))
	);
	
	Handle<Function> f = compile(def);
	ValueHandle ret = snow::call(NULL, f, 0);
	
	TEST_EQ(integer(ret), 579);
}

TEST_CASE(simple_closure) {
	HandleScope _;

	RefPtr<FunctionDefinition> def = _function(
		_local_assign(_ident("a"), _lit_int(123)),
		_local_assign(_ident("func"),
			_function(
				_local_assign(_ident("b"), _lit_int(456)),
				_member_call(_ident("b"), _ident("+"), _seq(_ident("a")))
			)
		),
		_local_call(_ident("func"))
	);
	
	Handle<Function> f = compile(def);
	//dump_disasm(std::cout, f);
	ValueHandle ret = snow::call(NULL, f, 0);
	
	TEST_EQ(integer(ret), 579);
}

TEST_CASE(object_get) {
	HandleScope _;
	RefPtr<FunctionDefinition> def = _function(
		_member(_ident("obj"), _ident("member"))
	);
	def->arguments.push_back(_ident("obj"));
	
	Handle<Object> obj = gc_new<Object>();
	obj->set_raw_s("member", value(456LL));
	
	Handle<Function> f = compile(def);
	ValueHandle ret = snow::call(NULL, f, 1, obj.value());
	
	TEST_EQ(integer(ret), 456LL);
}

TEST_CASE(object_set) {
	HandleScope _;
	RefPtr<FunctionDefinition> def = _function(
		_member_assign(_ident("obj"), _ident("member"), _ident("value"))
	);
	def->arguments.push_back(_ident("obj"));
	def->arguments.push_back(_ident("value"));
	
	Handle<Object> obj = gc_new<Object>();
	Handle<Function> f = compile(def);
	ValueHandle ret = snow::call(NULL, f, 2, obj.value(), value(456LL));
	
	TEST_EQ(integer(obj->get_raw_s("member")), 456LL);
}

TEST_CASE(simple_loop) {
	HandleScope _;
	RefPtr<FunctionDefinition> def = _function(
		_local_assign(_ident("a"), _lit_int(0)),
		_loop(
			// condition:
			_member_call(_ident("a"), _ident("<"), _seq(_lit_int(1000))),
			// body:
			_local_assign(_ident("a"), _member_call(_ident("a"), _ident("+"), _seq(_lit_int(1))))
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
			_member_call(_ident("a"), _ident("="), _seq(_lit_str("This is a test"))),
			// body:
			_return(_lit_int(1))
		),
		_lit_int(0)
	);
	def->arguments.push_back(_ident("a"));
	
	Handle<Function> f = compile(def);
	TEST_EQ(snow::call(NULL, f, 0), value(0LL));
	TEST_EQ(snow::call(NULL, f, 1, gc_new<String>("This is a test")), value(1LL));
	TEST_EQ(snow::call(NULL, f, 1, value(123LL)), value(0LL));
}

TEST_CASE(unless_condition) {
	HandleScope _;
	RefPtr<FunctionDefinition> def = _function(
		_if(
			// condition:
			_member_call(_ident("a"), _ident("="), _seq(_lit_str("A test, this is"))),
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
	TEST_EQ(snow::call(NULL, f, 1, gc_new<String>("A test, this is")), value(0LL));
	TEST_EQ(snow::call(NULL, f, 1, value(123LL)), value(1LL));
}

TEST_CASE(if_else_condition) {
	HandleScope _;
	// also tests the result of if-expressions
	RefPtr<FunctionDefinition> def = _function(
		_if_else(
			// condition:
			_member_call(_ident("a"), _ident("="), _seq(_lit_str("This is a test"))),
			// if true:
			_lit_int(123LL),
			// if false:
			_lit_int(456LL)
		)
	);
	def->arguments.push_back(_ident("a"));
	
	Handle<Function> f = compile(def);
	TEST_EQ(snow::call(NULL, f, 0), value(456LL));
	TEST_EQ(snow::call(NULL, f, 1, gc_new<String>("This is a test")), value(123LL));
	TEST_EQ(snow::call(NULL, f, 1, gc_new<String>("LOL")), value(456LL));
}

TEST_CASE(unless_else_condition) {
	HandleScope _;
	// also tests the result of unless-expressions
	RefPtr<FunctionDefinition> def = _function(
		_if_else(
			// condition:
			_member_call(_ident("a"), _ident("="), _seq(_lit_str("A test, this is"))),
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
	TEST_EQ(snow::call(NULL, f, 1, gc_new<String>("A test, this is")), value(771LL));
	TEST_EQ(snow::call(NULL, f, 1, gc_new<String>("F00B4RZ!")), value(999LL));
}

TEST_CASE(logical_and) {
	HandleScope _;
	
	RefPtr<FunctionDefinition> def = _function(
		_logical_and(
			_ident("a"),
			_local_assign(_ident("b"), _lit_int(123LL))
		),
		_return(_ident("b"))
	);
	def->arguments.push_back(_ident("a"));
	
	Handle<Function> f = compile(def);
	TEST_EQ(snow::call(NULL, f, 1, value(false)), nil());
	TEST_EQ(snow::call(NULL, f, 1, nil()), nil());
	TEST_EQ(snow::call(NULL, f, 1, value(0LL)), value(123LL));
	TEST_EQ(snow::call(NULL, f, 1, value(456LL)), value(123LL));
}

TEST_CASE(logical_or) {
	HandleScope _;
	
	RefPtr<FunctionDefinition> def = _function(
		_logical_or(
			_ident("a"),
			_ident("b")
		)
	);
	def->arguments.push_back(_ident("a"));
	def->arguments.push_back(_ident("b"));
	
	Handle<Function> f = compile(def);
	TEST_EQ(snow::call(NULL, f, 2, value(false), value(123LL)), value(123LL));
	TEST_EQ(snow::call(NULL, f, 2, nil(), value(123LL)), value(123LL));
	TEST_EQ(snow::call(NULL, f, 2, value(123LL), nil()), value(123LL));
	TEST_EQ(snow::call(NULL, f, 2, nil(), value(false)), value(false));
}

TEST_CASE(logical_xor) {
	HandleScope _;
	RefPtr<FunctionDefinition> def = _function(
		_logical_xor(
			_ident("a"),
			_ident("b")
		)
	);
	def->arguments.push_back(_ident("a"));
	def->arguments.push_back(_ident("b"));
	
	Handle<Function> f = compile(def);
	TEST_EQ(snow::call(NULL, f, 2, value(false), value(123LL)), value(123LL));
	TEST_EQ(snow::call(NULL, f, 2, value(123LL), nil()), value(123LL));
	TEST_EQ(snow::call(NULL, f, 2, nil(), nil()), value(false));
	TEST_EQ(snow::call(NULL, f, 2, value(false), value(false)), value(false));
	TEST_EQ(snow::call(NULL, f, 2, nil(), value(false)), value(false));
	TEST_EQ(snow::call(NULL, f, 2, value(false), nil()), value(false));
}

TEST_CASE(logical_not) {
	HandleScope _;
	RefPtr<FunctionDefinition> def = _function(_logical_not(_ident("a")));
	def->arguments.push_back(_ident("a"));
	Handle<Function> f = compile(def);
	
	TEST_EQ(snow::call(NULL, f, 1, nil()), value(true));
	TEST_EQ(snow::call(NULL, f, 1, value(false)), value(true));
	TEST_EQ(snow::call(NULL, f, 1, value(true)), value(false));
	TEST_EQ(snow::call(NULL, f, 1, value(42LL)), value(false));
	TEST_EQ(snow::call(NULL, f, 1, value(-3LL)), value(false));
	TEST_EQ(snow::call(NULL, f, 1, gc_new<String>("LAWL")), value(false));
}

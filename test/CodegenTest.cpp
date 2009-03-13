#include "test.h"
#include "lib/Function.h"
#include "Codegen.h"
#include "ASTNode.h"
#include "lib/Runtime.h"
using namespace snow;

TEST_SUITE(Codegen);

static Handle<Function> compile(RefPtr<Codegen> codegen) {
	SymbolTable table;
	table["snow_eval_truth"] = (void*)snow::eval_truth;
	table["snow_call"] = (void*)snow::call;
	table["snow_get"] = (void*)snow::get;
	table["snow_set"] = (void*)snow::set;
	table["snow_enter_scope"] = (void*)snow::enter_scope;
	table["snow_leave_scope"] = (void*)snow::leave_scope;
	table["snow_get_local"] = (void*)snow::get_local;
	
	RefPtr<CompiledCode> cc = codegen->compile();
	cc->export_symbols(table);
	cc->link(table);
	cc->make_executable();
	
	return new Function(cc->function());
}

TEST_CASE(simple_add) {
	using namespace snow::ast;
	
	RefPtr<FunctionDefinition> def = new FunctionDefinition(
		new Assignment(new Identifier("a"), new Literal("123", Literal::INTEGER_TYPE)),
		new Assignment(new Identifier("b"), new Literal("456", Literal::INTEGER_TYPE)),
		new Call(new Identifier("a"), new Identifier("+"), new Sequence(new Identifier("b")))
	);
	
	RefPtr<Codegen> codegen = Codegen::create(*def);
	
	Handle<Function> f = compile(codegen);
	
	ValueHandle ret = snow::call(NULL, f, 0);
	
	TEST_EQ(integer(ret.value()), 579);
}

#include <sys/mman.h>
#include "asmtest.h"
#include "x86_64/Operand.h"
#include "x86_64/Assembler.h"
#include "x86_64/Codegen.h"
#include "ASTNode.h"
#include "Realizer.h"
#include "CompiledCode.h"
#include "Linker.h"
#include "lib/IncrementalAlloc.h"
#include "Util.h"
#include <iostream>
using namespace std;

#include "lib/Runtime.h"
#include "lib/Object.h"

#define __ masm.

using namespace snow;
//using namespace snow::x86_64;

void test_codegen() {
	SymbolTable table;
	table["snow_create_object"] = (void*)snow::create_object;
	table["snow_send"] = (void*)snow::send;
	table["snow_call"] = (void*)snow::call;
	table["snow_destroy"] = (void*)snow::destroy;

	x86_64::Codegen masm;

	Scope s = __ function_entry(3);
	auto l1 = Codegen::Local(0, "hejsa");
	__ get_argument(0, l1);
	auto l2 = Codegen::Local(1);
	__ get_argument(1, l2);

	__ set_argument(0, l1);
	__ set_argument(1, "+");
	__ set_argument(2, 1);
	__ set_argument(3, l2);
	auto retval = Codegen::Local(2);
	__ call("snow_send", retval);

	__ set_argument(0, l1);
	__ call("snow_destroy");
	__ set_argument(0, l2);
	__ call("snow_destroy");

//	__ debug_break();
	__ set_return(retval);
	__ function_return();


	RefPtr<CompiledCode> code = __ compile();
	code->export_symbols(table);
	code->link(table);

	code->make_executable();

	print_mem(code->code(), &code->code()[code->size()]);
	printf("code is at 0x%llx\n", (uint64_t)code->code());

	VALUE(*entry)(VALUE a, VALUE b) = (VALUE(*)(VALUE, VALUE))code->code();
	printf("add: %lld\n", integer(entry(value(-67LL), value(-45LL))));
}

void test_ast() {
	using namespace snow::ast;
	
	SymbolTable table;
	table["muh"] = (void*)create_object;
	table["snow_create_stack_frame"] = (void*)snow::create_stack_frame;
	
	RefPtr<ast::Scope> scope = new ast::Scope;
	scope->add(new Assignment(new Identifier("a"), new Literal("123", Literal::INTEGER_TYPE)));
	scope->add(new Assignment(new Identifier("b"), new Literal("567", Literal::INTEGER_TYPE)));
	scope->add(new Call(
		new Send(
			new Call(new Identifier("a")),
			new Identifier("+")
		),
		new Sequence(new Identifier("b"))
	));
	
	Realizer r;
	RefPtr<CompiledCode> cc = r.realize(*scope);
	cc->export_symbols(table);
	cc->link(table);
	cc->make_executable();
	
	VALUE global_scope = create_object();
	
	printf("code is at: 0x%llx\n", (uint64_t)cc->code());
	VALUE ret = cc->function()(global_scope, 0, NULL);
	printf("returned: 0x%llx\n", (uint64_t)ret);
}

int main (int argc, char const *argv[])
{
	test_ast();

	return 0;
}

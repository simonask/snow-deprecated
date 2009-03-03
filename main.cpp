#include <sys/mman.h>
#include "asmtest.h"
#include "x86_64/Operand.h"
#include "x86_64/Assembler.h"
#include "x86_64/Codegen.h"
#include "ASTNode.h"
#include "CompiledCode.h"
#include "Linker.h"
#include "lib/IncrementalAlloc.h"
#include "Util.h"
#include <iostream>
using namespace std;

#include "lib/Runtime.h"
#include "lib/Object.h"
#include "lib/Array.h"
#include "lib/RuntimeMacros.h"

#define __ masm.

using namespace snow;
//using namespace snow::x86_64;

void test_codegen() {/*
	SymbolTable table;
	table["snow_create_object"] = (void*)snow::create_object;
	table["snow_send"] = (void*)snow::send;
	table["snow_call"] = (void*)snow::call;
	table["snow_destroy"] = (void*)snow::destroy;

	x86_64::Codegen masm;

	__ function_entry(3);
	auto l1 = Scope::Local(0, "hejsa");
	__ get_argument(0, l1);
	auto l2 = Scope::Local(1);
	__ get_argument(1, l2);

	__ set_argument(0, l1);
	__ set_argument(1, "+");
	__ set_argument(2, 1);
	__ set_argument(3, l2);
	auto retval = Scope::Local(2);
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
	printf("add: %lld\n", integer(entry(value(-67LL), value(-45LL))));*/
}

static VALUE global_puts(Scope* scope) {
	for (uint64_t i = 0; i < NUM_ARGS; ++i) {
		printf("%s\n", value_to_string(ARGS[i]));
	}
	return nil();
}

void test_ast() {
	using namespace snow::ast;
	
	SymbolTable table;
	table["snow_eval_truth"] = (void*)snow::eval_truth;
	table["snow_call"] = (void*)snow::call;
	table["snow_call_method"] = (void*)snow::call_method;
	table["snow_send"] = (void*)snow::send;
	table["snow_enter_scope"] = (void*)snow::enter_scope;
	table["snow_leave_scope"] = (void*)snow::leave_scope;
	table["snow_get_local"] = (void*)snow::get_local;
	
	RefPtr<FunctionDefinition> scope = new FunctionDefinition;
	scope->arguments.push_back(new Identifier("c"));
	scope->add(new Call(new Identifier("puts"), new Sequence(new Literal("TAP 0", Literal::STRING_TYPE))));
	
	scope->add(new Call(new Identifier("puts"), new Sequence(new Identifier("c"))));
	scope->add(new Assignment(new Identifier("a"), new Literal("123", Literal::INTEGER_TYPE)));
	scope->add(new Assignment(new Identifier("b"), new Literal("567", Literal::INTEGER_TYPE)));
	scope->add(new Assignment(
			new Identifier("d"),
			new MethodCall(
				new Identifier("a"),
				new Identifier("+"),
				new Sequence(new Identifier("b"))
			)
		)
	);
	
	scope->add(new Call(new Identifier("puts"), new Sequence(new Literal("TAP 1", Literal::STRING_TYPE))));
	
	scope->add(new Assignment(
			new Identifier("d"),
			new MethodCall(
				new Identifier("d"),
				new Identifier("+"),
				new Sequence(new Identifier("c"))
			)
		)
	);
	
	scope->add(new Call(new Identifier("puts"), new Sequence(new Literal("TAP 2", Literal::STRING_TYPE))));
	
	scope->add(new Assignment(
			new Identifier("e"),
			new Literal("0", Literal::INTEGER_TYPE)
		)
	);
	
	scope->add(new Call(new Identifier("puts"), new Sequence(new Literal("TAP 3", Literal::STRING_TYPE))));
	
	scope->add(new Loop(
			new MethodCall(
				new Identifier("e"),
				new Identifier("<"),
				new Sequence(new Identifier("d"))
			),
			new Sequence(
				new Call(
					new Identifier("puts"),
					new Sequence(new Identifier("e"))
				),
				new Assignment(new Identifier("e"),
					new MethodCall(
						new Identifier("e"),
						new Identifier("+"),
						new Sequence(new Literal("1", Literal::INTEGER_TYPE))
					)
				)
			)
		)
	);
	
	scope->add(new Call(new Identifier("puts"), new Sequence(new Literal("TAP 4", Literal::STRING_TYPE))));
	
	scope->add(new IfCondition(
			new MethodCall(
				new Identifier("e"),
				new Identifier("="),
				new Sequence(new Identifier("d"))
			),
			new Call(
				new Identifier("puts"),
				new Sequence(new Literal("HELLO WORLD!", Literal::STRING_TYPE))
			)
		)
	);
	
	scope->add(new Call(new Identifier("puts"), new Sequence(new Literal("TAP 5", Literal::STRING_TYPE))));
	
	scope->add(new IfElseCondition(
			new MethodCall(
				new Identifier("e"),
				new Identifier("="),
				new Sequence(new Identifier("c"))
			),
			new Call(
				new Identifier("puts"),
				new Sequence(new Literal("YES", Literal::STRING_TYPE))
			),
			new Call(
				new Identifier("puts"),
				new Sequence(new Literal("NO", Literal::STRING_TYPE))
			)
		)
	);
	
	scope->add(new Call(new Identifier("puts"), new Sequence(new Literal("TAP 6", Literal::STRING_TYPE))));
	
	scope->add(new Return(new Identifier("d")));
	scope->add(new MethodCall(new Identifier("self"), new Identifier("puts"), new Sequence(new Literal("Should not be reached", Literal::STRING_TYPE))));
	
	RefPtr<Codegen> codegen = Codegen::create(*scope);
	RefPtr<CompiledCode> cc = codegen->compile();
	
	cc->export_symbols(table);
	cc->link(table);
	cc->make_executable();
	
	Handle<Scope> global_scope = new Scope(new Function);
	global_scope->set_local("puts", create_function(global_puts));
	
	printf("code is at: 0x%llx\n", (uint64_t)cc->code());
	Handle<Function> f = new Function(cc->function());
	f->set_parent_scope(global_scope);
	
	VALUE ret = f->call(nil(), new Array((VALUE[]){value(8LL), value(6LL),value(7LL), value(5LL), value(5LL)}, 5));
	printf("returned: %s\n", value_to_string(ret));
}

void test_sib() {
	using namespace snow::x86_64;
	x86_64::Assembler masm;
	
	__ enter(32);
	__ mov(3LL, rax);
	__ mov(SIB(rdi, rax, SIB::SCALE_8), rbx);
	__ mov(rbx, rax);
	__ debug_break();
	__ leave();
	__ ret();
	
	RefPtr<CompiledCode> cc = masm.compile();
	cc->make_executable();
	
	int64_t* array = new int64_t[10];
	for (int i = 0; i < 10; ++i) {
		array[i] = 123 + i;
	}
	
	debug("array is at 0x%llx", array);
	debug("code is at 0x%llx", cc->function());
	Function f = cc->function();
	int64_t ret = (int64_t)f.call(array, new Array);
	
	delete[] array;
	
	debug("returned: %lld", ret);
}

int main (int argc, char const *argv[])
{
	//Garbage::collect();
	test_ast();
	//test_sib();
	TempAllocator<ast::Node>::flush();
	//Garbage::collect();
	
	return 0;
}

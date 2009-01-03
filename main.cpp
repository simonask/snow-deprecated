#include <sys/mman.h>
#include "asmtest.h"
#include "x86_64/Operand.h"
#include "x86_64/Assembler.h"
#include "x86_64/Codegen.h"
#include "CompiledCode.h"
#include "Linker.h"
#include "lib/IncrementalAlloc.h"
#include "Util.h"
#include <iostream>
using namespace std;

#include "lib/Runtime.h"
#include "lib/Object.h"

#define __ masm.

using namespace snot;
using namespace snot::x86_64;

void say_hello() {
	printf("HELLO WORLD! %lx\n", (void*)say_hello);
}

void val_to_int(x86_64::Assembler& masm, const Register& reg) {
	__ shr(reg);
}

void int_to_val(x86_64::Assembler& masm, const Register& reg) {
	__ shl(reg);
	__ inc(reg);
}

void if_int(x86_64::Assembler& masm, const Register& reg, Label& no) {
	__ clear(rax);
	__ incb(rax);
	__ bin_and(reg, rax);
	__ j(CC_ZERO, no);
}

VALUE object_initialize(VALUE self, uint64_t num_args, VALUE* args) {
	printf("initializing 0x%lx, with %lu args\n", self, num_args);
	char c = ((char*)NULL)[0];
	return self;
}

VALUE object_real_object(VALUE self, uint64_t num_args, VALUE* args) {
	printf("THIS IS A REAL OBJECT: 0x%lx\n", self);
	return self;
}

int main (int argc, char const *argv[])
{
	SymbolTable table;
	table["say_hello"] = (void*)say_hello;
	table["snot_create_object"] = (void*)snot::create_object;
	table["snot_send"] = (void*)snot::send;
	table["snot_call"] = (void*)snot::call;
	table["snot_destroy"] = (void*)snot::destroy;
	
	x86_64::Codegen masm;
	
	Scope s = __ function_entry(3);
	auto l1 = s.local("hejsa");
	__ get_argument(0, l1);
	auto l2 = s.local();
	__ get_argument(1, l2);
	
	__ set_argument(0, l1);
	__ set_argument(1, "+");
	__ set_argument(2, 1);
	__ set_argument(3, l2);
	auto retval = s.local();
	__ call("snot_send", retval);
	
	__ set_argument(0, l1);
	__ call("snot_destroy");
	__ set_argument(0, l2);
	__ call("snot_destroy");
	
//	__ debug_break();
	__ set_return(retval);
	__ function_return();
	
	
	CompiledCode code = __ compile();
	Linker::register_symbols(code, table);
	Linker::link(code, table);
	
	code.make_executable();
	
	print_mem(code.code(), &code.code()[code.size()]);
	printf("code is at 0x%lx\n", code.code());
	
	Object* obj = new Object;
	VALUE func = snot::create_function(object_initialize);
	obj->set("initialize", func);
	obj->set("real_object", snot::create_function(object_real_object));
	
	VALUE(*entry)(VALUE a, VALUE b) = (VALUE(*)(VALUE, VALUE))code.code();
	printf("add: %d\n", integer(entry(value(-67LL), value(-45LL))));

	return 0;
}

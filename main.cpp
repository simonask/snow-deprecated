#include <sys/mman.h>
#include "asmtest.h"
#include "x86_64/Operand.h"
#include "x86_64/Assembler.h"
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

CompiledCode define_function(const std::string& name) {
	x86_64::Assembler masm;
	__ define_symbol(name);
	__ enter(16);
	
	__ bin_xor(rdi, rdi); // NULL prototype
	__ call("snot_create_object");
	__ mov(rax, Address(rbp, -8));
	
	__ mov(Address(rbp, -8), rdi);
	__ clear(rax);
	__ call("snot_call");
	__ mov(rax, Address(rbp, -8));
	
	__ mov(Address(rbp, -8), rdi);
	__ mov("initialize", rsi);
	__ mov(3, rdx);
	__ clear(rax);
	__ call("snot_send");
	
	__ clear(rax);
	__ add(56, rax);
	__ debug_break();
	__ leave();
	__ ret();
	return masm.compile();
}

int main (int argc, char const *argv[])
{
	SymbolTable table;
	table["say_hello"] = (void*)say_hello;
	table["snot_create_object"] = (void*)snot::create_object;
	table["snot_send"] = (void*)snot::send;
	table["snot_call"] = (void*)snot::call;
	
	CompiledCode code = define_function("hejsa");
	Linker::register_symbols(code, table);
	Linker::link(code, table);
	
	code.make_executable();
	
	print_mem(code.code(), &code.code()[code.size()]);
	print_mem(simple_ret, simple_loop);
	printf("code is at 0x%lx\n", code.code());
	
	int(*func)() = (int(*)())code.code();
	printf("add: %d\n", func());

	return 0;
}

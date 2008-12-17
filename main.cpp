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

#define __ masm.

using namespace snot;
using namespace snot::x86_64;

void say_hello() {
	printf("HELLO WORLD! %lx\n", (void*)say_hello);
}

CompiledCode define_function(const std::string& name) {
	x86_64::Assembler masm;
	__ define_symbol(name);
	__ enter();
	__ call("say_hello");
	__ mov(Immediate(15), rax);
	__ debug_break();
	__ debug_break();
	__ leave();
	__ ret();
	return masm.compile();
}

int main (int argc, char const *argv[])
{
	SymbolTable table;
	table["say_hello"] = (void*)say_hello;
	
	CompiledCode code = define_function("hejsa");
	Linker::register_symbols(code, table);
	Linker::link(code, table);
	
	code.make_executable();
	
	print_mem(code.code(), &code.code()[code.size()]);
	print_mem(simple_ret, simple_loop);
	printf("code is at 0x%lx\n", code.code());
	
	int(*func)() = (int(*)())code.code();
	printf("add: %d\n", func());
	char* br = NULL;
	br[0] = '0';

	return 0;
}

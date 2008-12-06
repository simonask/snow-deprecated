#include <sys/mman.h>
#include "asmtest.h"
#include "Operand-x86_64.h"
#include "Assembler-x86_64.h"
using namespace x86_64;

template<typename X, typename Y>
void print_mem(X* start, Y* end) {
	unsigned char* i = (unsigned char*)start;
	unsigned char* j = (unsigned char*)end;
	for (; i < j; ++i) {
		printf("0x%.2x ", *i);
	}
	puts("");
}

void say_hello() {
	printf("HELLO WORLD!\n");
}

int main (int argc, char const *argv[])
{
	Assembler masm;
	#define __ masm.
	
	__ enter(Immediate(0));
	__ call("say_hello");
	__ call("count_up");
	__ leave();
	__ ret();
	
	__ define_symbol("count_up");
	__ enter(Immediate(0));
	__ bin_xor(rax, rax);
	__ mov(Immediate(200), rbx);
	Assembler::Label loop_cond, loop_exit;
	__ bind(loop_cond);
	__ cmp(rax, rbx);
	__ j(CC_EQUAL, loop_exit);
	__ inc(rax);
	__ jmp(loop_cond);
	__ bind(loop_exit);
	__ leave();
	__ ret();
	
	unsigned char* code = (unsigned char*)valloc(masm.length());
	Assembler::SymbolTable table;
	table["say_hello"] = Assembler::Symbol((void*)say_hello);
	masm.compile_to(code, table);
	
	puts("generated:");
	print_mem(code, &code[masm.length()]);
	
	mprotect(code, masm.length(), PROT_EXEC);
	
	int(*func)(int a, int b) = (int(*)(int, int))code;
	printf("add: %d\n", func(6772, 2123));

	return 0;
}

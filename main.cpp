#include "Instruction.h"
#include "CodeBuffer.h"
#include <sys/mman.h>
#include "asmtest.h"
#include "Operand-x86_64.h"
#include "Assembler-x86_64.h"
using namespace x86_64;

void print_bytes(const ByteString& str) {
	for (int i = 0; i < str.length(); ++i) {
		printf("0x%.2x ", str.get(i));
	}
}

template<typename X, typename Y>
void print_mem(X* start, Y* end) {
	unsigned char* i = (unsigned char*)start;
	unsigned char* j = (unsigned char*)end;
	for (; i < j; ++i) {
		printf("0x%.2x ", *i);
	}
	puts("");
}

int main (int argc, char const *argv[])
{
	ByteString str;
	Assembler masm(str);
	Assembler::Label loop_cond;
	Assembler::Label loop_exit;
	masm.enter(Immediate(0));
	masm.bin_xor(rax, rax);
	masm.mov(Immediate(201), rbx);
	masm.bind(loop_cond);
	masm.cmp(rax, rbx);
	masm.j(CC_EQUAL, loop_exit);
	masm.inc(rax);
	masm.jmp(loop_cond);
	masm.bind(loop_exit);
	masm.leave();
	masm.ret();
	
	unsigned char* code = (unsigned char*)valloc(str.length());
	str.copyTo(code);
	
	puts("generated:");
	print_mem(code, &code[str.length()]);
	
	puts("reference:");
	print_mem(simple_loop, movregreg);
	
	mprotect(code, str.length(), PROT_EXEC);
	printf("executing code at 0x%x...\n", code);
	
	int(*func)(int a, int b) = (int(*)(int, int))code;
	printf("add: %d\n", func(6772, 2123));

	return 0;
}

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
}



int main (int argc, char const *argv[])
{
	Assembler masm;
	long long n = 125;
//	masm.push(rbp);
//	masm.mov(rsp, rbp);
	masm.mov(rdi, rax);
	masm.add(rsi, rax);
//	masm.leave();
	masm.ret();
	ByteString str = masm.code();
	unsigned char data[str.length()];
	str.copyTo(data);
	puts("push:");
	print_bytes(str);
	puts("");
	
	puts("movregreg:");
	print_mem(movregreg, pushpopmem);
	puts("");
	puts("pushpopmem:");
	print_mem(pushpopmem, pushpop);
	puts("");
	puts("pushpop:");
	print_mem(pushpop, end);
	puts("");
	
	unsigned char* code = (unsigned char*)valloc(str.length());
	str.copyTo(code);
	mprotect(code, str.length(), PROT_EXEC);
	printf("executing code at 0x%x...\n", code);
	
	int(*func)(int a, int b) = (int(*)(int, int))code;
	printf("add: %d\n", func(6772, 2123));

	return 0;
}

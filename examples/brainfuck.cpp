#include "Assembler-x86_64.h"
#include "ByteString.h"
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#include <queue>

using namespace x86_64;

template <typename T>
void store_ptr(Assembler& m, T* ptr, const Address& addr) {
	m.mov(Immediate(&ptr, sizeof(T*)), rax);
	m.mov(rax, addr);
}

static bool moved_since_last_load = true;

void load_cell_addr_to_rax(Assembler& m, bool force = false) {
	if (moved_since_last_load || force) {
		m.mov(Address(rbp, -8), rax);
		m.add(Address(rbp, -16), rax);
		moved_since_last_load = false;
	}
}

void inc(Assembler& m) {
	load_cell_addr_to_rax(m);
	m.incb(Address(rax));
}

void dec(Assembler& m) {
	load_cell_addr_to_rax(m);
	m.decb(Address(rax));
}

void move_left(Assembler& m) {
	m.dec(Address(rbp, -16));
	moved_since_last_load = true;
}

void move_right(Assembler& m) {
	m.inc(Address(rbp, -16));
	moved_since_last_load = true;
}

void put_char(Assembler& m) {
	load_cell_addr_to_rax(m);
	m.mov(Address(rax), rdi);
	m.call(Address(rbp, -24));
	moved_since_last_load = true;
}

void get_char(Assembler& m) {
	load_cell_addr_to_rax(m);
	m.mov(Address(rax), rdi);
	m.call(Address(rbp, -32));
	m.mov(rax, rbx);
	load_cell_addr_to_rax(m, true);
	m.mov(rbx, Address(rax));	// XXX: moves 8 bytes, should be 1
}


static std::queue<Assembler::Label> jump_forward_labels;
static std::queue<Assembler::Label> jump_backward_labels;

void jump_forward(Assembler& m) {
	Assembler::Label front;
	Assembler::Label back;
	
	load_cell_addr_to_rax(m);
	m.bin_xor(rbx, rbx);
	m.cmpb(Address(rax), rbx);
	m.j(CC_EQUAL, back);
	m.bind(front);
	
	jump_forward_labels.push(front);
	jump_backward_labels.push(back);
}

void jump_backward(Assembler& m) {
	if (jump_forward_labels.empty() || jump_backward_labels.empty()) {
		fprintf(stderr, "ERROR: Unmatched ]\n");
	} else {
		Assembler::Label front = jump_forward_labels.front();
		jump_forward_labels.pop();
		Assembler::Label back = jump_backward_labels.front();
		jump_backward_labels.pop();
		
		load_cell_addr_to_rax(m);
		m.bin_xor(rbx, rbx);
		m.cmpb(Address(rax), rbx);
		m.j(CC_NOT_EQUAL, front);
		m.bind(back);
	}
}

const char* bf = "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++>++++++[<.>-]>++++++++++.";
const char* hello = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.";

int main (int argc, char const *argv[])
{
	ByteString code;
	Assembler m(code);
	
	unsigned char* buffer = (unsigned char*)malloc(1 << 16);
	memset(buffer, 0, (1 << 16));
	
	m.enter(Immediate(32));
	store_ptr(m, buffer, Address(rbp, -8));
	m.mov(Immediate(0), Address(rbp, -16));
	store_ptr(m, putchar, Address(rbp, -24));
	store_ptr(m, getchar, Address(rbp, -32));
	
	const char* input = hello;
	
	size_t len = strlen(input);
	int i;
	for (i = 0; i < len; ++i) {
		switch (input[i]) {
		case '+':
			inc(m);
			break;
		case '-':
			dec(m);
			break;
		case '>':
			move_right(m);
			break;
		case '<':
			move_left(m);
			break;
		case '.':
			put_char(m);
			break;
		case ',':
			get_char(m);
			break;
		case '[':
			jump_forward(m);
			break;
		case ']':
			jump_backward(m);
			break;
		default:
			break;
		}
	}

	m.bin_xor(rax, rax);
	m.leave();
	m.ret();
	
	unsigned char* raw_code = (unsigned char*)valloc(code.length());
	code.copyTo(raw_code);
	mprotect(raw_code, code.length(), PROT_EXEC);
	
	void(*func)() = (void(*)())raw_code;
	func();
	
	
	for (int i = 0; i < 10; ++i) {
		printf("0x%.2x ", buffer[i]);
	}
	puts("");
	
	free(raw_code);
	free(buffer);
	return 0;
}
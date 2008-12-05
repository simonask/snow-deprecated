#include "Instruction.h"
#include "Operand-x86_64.h"

using namespace x86_64;

ByteString PushInstruction::compile() const {
	ByteString b;
	const Register* reg = dynamic_cast<const Register*>(&m_Operand);
	if (reg) {
		if (reg->extended()) {
			char rex = 0x41;
			b << rex;
		}
		char opcode = 0x50;
		opcode += reg->code();
		b << opcode;
		return b;
	}
}

size_t PushInstruction::length() const {
	const Register* reg = dynamic_cast<const Register*>(&m_Operand);
	return (reg && reg->code()) ? 2 : 1;
}

ByteString PopInstruction::compile() const {
	ByteString b;
	const Register* reg = dynamic_cast<const Register*>(&m_Operand);
	if (reg) {
		if (reg->extended()) {
			char rex = 0x41;
			b << rex;
		}
		char opcode = 0x58;
		opcode += reg->code();
		b << opcode;
		return b;
	}
}

size_t PopInstruction::length() const {
	const Register* reg = dynamic_cast<const Register*>(&m_Operand);
	return (reg && reg->code()) ? 2 : 1;
}

ByteString RetInstruction::compile() const {
	return ByteString("\xc3");
}

size_t RetInstruction::length() const {
	return 1;
}

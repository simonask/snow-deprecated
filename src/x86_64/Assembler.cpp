#ifndef ASSEMBLER_X86_64_CPP_DNNNABGU
#define ASSEMBLER_X86_64_CPP_DNNNABGU

#include "x86_64/Assembler.h"
#include <iostream>
using namespace std;

#define u (unsigned char)
#define U (unsigned char*)

namespace snot {
namespace x86_64 {
	unsigned char Assembler::rex_for_operands(const Register& reg, const Register& rm) {
		int flags = rex_for_operand(rm);
		if (reg.extended())
			flags |= REX_EXTEND_REG;
		return flags;
	}
	
	unsigned char Assembler::rex_for_operands(const Register& reg, const Address& rm) {
		int flags = rex_for_operands(reg, rm.reg());
		if (rm.wide())
			flags |= REX_WIDE_OPERAND;
		return flags;
	}
	
	unsigned char Assembler::rex_for_operand(const Register& rm_or_opcode_reg) {
		int flags = NO_REX;
		if (rm_or_opcode_reg.extended())
			flags |= REX_EXTEND_RM;
		return flags;
	}
	
	unsigned char Assembler::rex_for_operand(const Address& rm) {
		int flags = rex_for_operand(rm.reg());
		if (rm.wide())
			flags |= REX_WIDE_OPERAND;
		return flags;
	}
	
	void Assembler::emit_immediate(const Immediate& imm, size_t bytes) {
		long long value = imm.data();
		unsigned char* data = reinterpret_cast<unsigned char*>(&value);
		for (int i = 0; i < bytes; ++i) {
			emit(data[i]);
		}
	}
	
	void Assembler::emit_modrm(unsigned char mod, unsigned char reg, unsigned char rm) {
		mod = (mod << 6) & 0xc0;
		reg = (reg << 3) & 0x38;
		rm &= 0x07;
		emit(mod | reg | rm);
	}
	
	void Assembler::emit_modrm(const Register& rm, unsigned char opcode_ext) {
		emit_modrm(RM_REGISTER, opcode_ext, rm.code());
	}
	
	void Assembler::emit_modrm(const Register& reg, const Register& rm) {
		emit_modrm(RM_REGISTER, reg.code(), rm.code());
	}
	
	void Assembler::emit_modrm(const Address& rm, unsigned char opcode_ext) {
		emit_modrm(mod_for_address(rm), opcode_ext, rm.reg().code());
		emit_displacement(rm);
	}
	
	void Assembler::emit_modrm(const Register& reg, const Address& rm) {
		emit_modrm(mod_for_address(rm), reg.code(), rm.reg().code());
		emit_displacement(rm);
	}
	
	void Assembler::emit_displacement(const Address& addr) {
		switch (mod_for_address(addr)) {
			case RM_ADDRESS_DISP8:
				emit(addr.offset());
				break;
			case RM_ADDRESS_DISP32:
				emit_immediate(Immediate((int32_t)addr.offset()));
				break;
		}
	}
	
	void Assembler::emit_label_ref(const Label& label) {
		if (label.bound()) {
			int offset = label.offset() - (this->offset() + 4);
			unsigned char* _offset = reinterpret_cast<unsigned char*>(&offset);
			for (int i = 0; i < 4; ++i) {
				emit(_offset[i]);
			}
		} else {
			m_UnboundLabelReferences.push_back(UnboundLabelReference(label, offset(), 4));
			for (int i = 0; i < 4; ++i) {
				emit(0x00);
			}
		}
	}
	
	void Assembler::emit_instr(unsigned char* opcodes, const Register& reg, const Register& rm, int extra_rex) {
		emit_rex(rex_for_operands(reg, rm) | extra_rex);
		unsigned char* ptr = opcodes;
		while (*ptr != u'\x00') {
			emit(*ptr++);
		}
		emit_modrm(reg, rm);
	}
	
	void Assembler::emit_instr(unsigned char* opcodes, const Register& reg, const Address& rm, int extra_rex) {
		emit_rex(rex_for_operands(reg, rm) | extra_rex);
		unsigned char* ptr = opcodes;
		while (*ptr != u'\x00') {
			emit(*ptr++);
		}
		emit_modrm(reg, rm);
	}
	
	void Assembler::emit_instr(unsigned char* opcodes, const Register& rm, unsigned char opcode_ext, int extra_rex) {
		emit_rex(rex_for_operand(rm) | extra_rex);
		unsigned char* ptr = opcodes;
		while (*ptr != u'\x00') {
			emit(*ptr++);
		}
		emit_modrm(rm, opcode_ext);
	}
	
	void Assembler::emit_instr(unsigned char* opcodes, const Address& rm, unsigned char opcode_ext, int extra_rex) {
		emit_rex(rex_for_operand(rm) | extra_rex);
		unsigned char* ptr = opcodes;
		while (*ptr != u'\x00') {
			emit(*ptr++);
		}
		emit_modrm(rm, opcode_ext);
	}
	
	void Assembler::emit_instr(unsigned char opcode, const Register& reg, const Register& rm, int extra_rex) {
		emit_rex(rex_for_operands(reg, rm) | extra_rex);
		emit(opcode);
		emit_modrm(reg, rm);
	}
	
	void Assembler::emit_instr(unsigned char opcode, const Register& reg, const Address& rm, int extra_rex) {
		emit_rex(rex_for_operands(reg, rm) | extra_rex);
		emit(opcode);
		emit_modrm(reg, rm);
	}
	
	void Assembler::emit_instr(unsigned char opcode, const Register& rm, unsigned char opcode_ext, int extra_rex) {
		emit_rex(rex_for_operand(rm) | extra_rex);
		emit(opcode);
		emit_modrm(rm, opcode_ext);
	}
	
	void Assembler::emit_instr(unsigned char opcode, const Address& rm, unsigned char opcode_ext, int extra_rex) {
		emit_rex(rex_for_operand(rm) | extra_rex);
		emit(opcode);
		emit_modrm(rm, opcode_ext);
	}
	
	Assembler::RM_MODE Assembler::mod_for_address(const Address& addr) {
		if (addr.offset() == 0)
			return RM_ADDRESS;
		else if (addr.offset() & 0xffffff00 != 0)
			return RM_ADDRESS_DISP32;
		else
			return RM_ADDRESS_DISP8;
	}
	
	void Assembler::add(const Immediate& src, const Register& dst) {
		emit_instr(0x81, dst);
		emit_immediate(src);
	}
	
	void Assembler::add(const Immediate& src, const Address& dst) {
		emit_instr(0x81, dst);
		emit_immediate(src);
	}
	
	void Assembler::add(const Register& src, const Register& dst) {
		emit_instr(0x01, src, dst);
	}
	
	void Assembler::add(const Register& src, const Address& dst) {
		emit_instr(0x01, src, dst);
	}

	void Assembler::add(const Address& src, const Register& dst) {
		emit_instr(0x03, dst, src, REX_WIDE_OPERAND);
	}
	
	void Assembler::bin_and(const Immediate& src, const Register& dst) {
		emit_instr(0x81, dst, 4);
		emit_immediate(src);
	}
	
	void Assembler::bin_and(const Immediate& src, const Address& dst) {
		emit_instr(0x81, dst, 4);
		emit_immediate(src);
	}
	
	void Assembler::bin_and(const Register& src, const Register& dst) {
		emit_instr(0x21, src, dst);
	}
	
	void Assembler::bin_and(const Register& src, const Address& dst) {
		emit_instr(0x21, src, dst);
	}

	void Assembler::bin_and(const Address& src, const Register& dst) {
		emit_instr(0x23, dst, src);
	}
	
	void Assembler::bswap(const Register& reg) {
		emit_rex(rex_for_operand(reg));
		emit(0x0f);
		emit(0xc8 + reg.code());
	}
	
	void Assembler::call(const Immediate& rel32off) {
		emit(0xe8);
		emit_immediate(rel32off, 4);
	}
	
	void Assembler::call(const Register& reg) {
		emit_instr(0xff, reg, 2);
	}
	
	void Assembler::call(const Address& addr) {
		emit_instr(0xff, addr, 2);
	}
	
	void Assembler::call(const Symbol& symb) {
		if (symb.external()) {
			mov(Immediate((long long)symb.address()), rax);
			call(rax);
		} else {
			call(Immediate(symb.offset() - (offset() + 5)));
		}
	}
	
	void Assembler::call(const std::string& symb) {
		mov(Immediate(0), rax);
		m_UnboundExternalSymbolReferences.push_back(UnboundExternalSymbolReference(symb, offset() - 8, 8));
		call(rax);
	}
	
	void Assembler::call_far(const Address& addr) {
		emit_instr(0xff, addr, 3);
	}
	
	void Assembler::cmov(Condition cc, const Register& src, const Register& dst) {
		unsigned char opcodes[2];
		opcodes[0] = 0x0f;
		opcodes[1] = 0x40 + cc;
		emit_instr(opcodes, dst, src);
	}
	
	void Assembler::cmov(Condition cc, const Address& src, const Register& dst) {
		unsigned char opcodes[2];
		opcodes[0] = 0x0f;
		opcodes[1] = 0x40 + cc;
		emit_instr(opcodes, dst, src);
	}
	
	void Assembler::cmp(const Immediate& left, const Register& right, bool single_byte) {
		emit_instr(single_byte ? 0x80 : 0x81, right, 7);
		emit_immediate(left, single_byte ? 1 : 4);
	}
	
	void Assembler::cmp(const Register& left, const Register& right, bool single_byte) {
		emit_instr(single_byte ? 0x38 : 0x39, right, left);
	}
	
	void Assembler::cmp(const Register& left, const Address& right, bool single_byte) {
		emit_instr(single_byte ? 0x3a : 0x3b, left, right);
	}
	
	void Assembler::cmp(const Address& left, const Register& right, bool single_byte) {
		emit_instr(single_byte ? 0x38 : 0x39, right, left);
	}
	
	void Assembler::dec(const Register& reg, bool single_byte) {
		emit_instr(single_byte ? 0xfe : 0xff, reg, 1);
	}
	
	void Assembler::dec(const Address& addr, bool single_byte) {
		emit_instr(single_byte ? 0xfe : 0xff, addr, 1);
	}
	
	void Assembler::div(const Register& reg) {
		emit_instr(0xf7, reg, 6);
	}
	
	void Assembler::div(const Address& addr) {
		emit_instr(0xf7, addr, 6);
	}
	
	void Assembler::enter(const Immediate& stack_size, const Immediate& nesting_level) {
		emit(0xc8);
		emit_immediate(stack_size, 2);
		emit_immediate(nesting_level, 1);
	}
	
	void Assembler::idiv(const Register& reg) {
		emit_instr(0xf7, reg, 7);
	}
	
	void Assembler::idiv(const Address& addr) {
		emit_instr(0xf7, addr, 7);
	}
	
	void Assembler::imul(const Register& src, const Register& dst) {
		emit_instr(U"\x0f\xaf", dst, src);
	}
	
	void Assembler::imul(const Address& src, const Register& dst) {
		emit_instr(U"\x0f\xaf", dst, src);
	}
	
	void Assembler::imul(const Immediate& imm, const Register& src, const Register& dst) {
		emit_instr(0x69, dst, src);
		emit_immediate(imm, 4);
	}
	
	void Assembler::imul(const Immediate& imm, const Address& src, const Register& dst) {
		emit_instr(0x69, dst, src);
		emit_immediate(imm, 4);
	}
	
	void Assembler::inc(const Register& reg, bool single_byte) {
		emit_instr(single_byte ? 0xfe : 0xff, reg);
	}
	
	void Assembler::inc(const Address& addr, bool single_byte) {
		emit_instr(single_byte ? 0xfe : 0xff, addr);
	}
	
	void Assembler::j(Condition cc, const Label& label) {
		emit(0x0f);
		emit(0x80 + cc);
		emit_label_ref(label);
	}
	
	void Assembler::j(Condition cc, const Immediate& rel32off) {
		emit(0x0f);
		emit(0x80 + cc);
		emit_immediate(rel32off, 4);
	}
	
	void Assembler::jmp(const Label& label) {
		emit(0xe9);
		emit_label_ref(label);
	}
	
	void Assembler::jmp(const Immediate& rel32off) {
		emit(0xe9);
		emit_immediate(rel32off, 4);
	}
	
	void Assembler::jmp(const Register& reg) {
		emit_instr(0xff, reg, 4);
	}
	
	void Assembler::jmp(const Address& addr) {
		emit_instr(0xff, addr, 4);
	}
	
	void Assembler::jmp_far(const Address& addr) {
		emit_instr(0xff, addr, 5);
	}
	
	void Assembler::lea(const Address& src, const Register& dst) {
		emit_instr(0x8d, dst, src);
	}
	
	void Assembler::lzcnt(const Register& src, const Register& dst) {
		emit_instr(U"\xf3\x0f\xbd", dst, src);
	}
	
	void Assembler::lzcnt(const Address& src, const Register& dst) {
		emit_instr(U"\xf3\x0f\xbd", dst, src);
	}
	
	void Assembler::mov(const Register& src, const Register& dst) {
		emit_instr(0x89, src, dst, REX_WIDE_OPERAND);
	}
	
	void Assembler::mov(const Register& src, const Address& dst) {
		emit_instr(0x89, src, dst, REX_WIDE_OPERAND);
	}
	
	void Assembler::mov(const Address& src, const Register& dst) {
		emit_instr(0x8b, dst, src, REX_WIDE_OPERAND);
	}
	
	void Assembler::mov(const Immediate& src, const Register& dst) {
		emit_rex(rex_for_operand(dst) | REX_WIDE_OPERAND);
		emit(0xb8 + dst.code());
		emit_immediate(src, 8);
	}
	
	void Assembler::mov(const Immediate& src, const Address& dst) {
		emit_instr(0xc7, dst);
		emit_immediate(src, 4);
	}
	
	void Assembler::mul(const Register& reg) {
		emit_instr(0xf7, reg, 4);
	}
	
	void Assembler::mul(const Address& addr) {
		emit_instr(0xf7, addr, 4);
	}
	
	void Assembler::neg(const Register& reg) {
		emit_instr(0xf7, reg, 3);
	}
	
	void Assembler::neg(const Address& addr) {
		emit_instr(0xf7, addr, 3);
	}
	
	void Assembler::bin_not(const Register& reg) {
		emit_instr(0xf7, reg, 2);
	}
	
	void Assembler::bin_not(const Address& addr) {
		emit_instr(0xf7, addr, 2);
	}
	
	void Assembler::bin_or(const Immediate& src, const Register& dst) {
		emit_instr(0x81, dst, 1);
		emit_immediate(src);
	}
	
	void Assembler::bin_or(const Immediate& src, const Address& dst) {
		emit_instr(0x81, dst, 1);
		emit_immediate(src);
	}
	
	void Assembler::bin_or(const Register& src, const Register& dst) {
		emit_instr(0x09, src, dst);
	}
	
	void Assembler::bin_or(const Register& src, const Address& dst) {
		emit_instr(0x09, src, dst);
	}

	void Assembler::bin_or(const Address& src, const Register& dst) {
		emit_instr(0x0a, dst, src);
	}
	
	void Assembler::pop(const Register& reg) {
		emit_rex(rex_for_operand(reg));
		emit(0x58 + reg.code());
	}
	
	void Assembler::pop(const Address& addr) {
		emit_instr(0x8f, addr);
	}
	
	void Assembler::popcnt(const Register& src, const Register& dst) {
		emit_instr(U"\xf3\x0f\xb8", dst, src);
	}
	
	void Assembler::popcnt(const Address& src, const Register& dst) {
		emit_instr(U"\xf3\x0f\xb8", dst, src);
	}
	
	void Assembler::push(const Register& reg) {
		emit_rex(rex_for_operand(reg));
		emit(0x50 + reg.code());
	}
	
	void Assembler::push(const Immediate& imm) {
		emit(0x68);
		emit_immediate(imm);
	}
	
	void Assembler::push(const Address& addr) {
		emit_instr(0xff, addr, 6);
	}
	
	void Assembler::rol(const Register& reg) {
		emit_instr(0xd1, reg);
	}
	
	void Assembler::rol(const Address& addr) {
		emit_instr(0xd1, addr);
	}
	
	void Assembler::rol(const Register& reg, const Immediate& imm) {
		emit_instr(0xc1, reg);
		emit_immediate(imm, 1);
	}
	
	void Assembler::rol(const Address& addr, const Immediate& imm) {
		emit_instr(0xc1, addr);
		emit_immediate(imm, 1);
	}
	
	void Assembler::ror(const Register& reg) {
		emit_instr(0xd1, reg);
	}
	
	void Assembler::ror(const Address& addr) {
		emit_instr(0xd1, addr);
	}
	
	void Assembler::ror(const Register& reg, const Immediate& imm) {
		emit_instr(0xc1, reg, 1);
		emit_immediate(imm, 1);
	}
	
	void Assembler::ror(const Address& addr, const Immediate& imm) {
		emit_instr(0xc1, addr, 1);
		emit_immediate(imm, 1);
	}
	
	void Assembler::shl(const Register& reg) {
		emit_instr(0xd1, reg, 4);
	}
	
	void Assembler::shl(const Address& addr) {
		emit_instr(0xf1, addr, 4);
	}
	
	void Assembler::shl(const Register& reg, const Immediate& imm) {
		emit_instr(0xc1, reg, 4);
		emit_immediate(imm, 1);
	}
	
	void Assembler::shl(const Address& addr, const Immediate& imm) {
		emit_instr(0xc1, addr, 4);
		emit_immediate(imm, 1);
	}
	
	void Assembler::shr(const Register& reg) {
		emit_instr(0xd1, reg, 5);
	}
	
	void Assembler::shr(const Address& addr) {
		emit_instr(0xd1, addr, 5);
	}
	
	void Assembler::shr(const Register& reg, const Immediate& imm) {
		emit_instr(0xc1, reg, 5);
		emit_immediate(imm, 1);
	}
	
	void Assembler::shr(const Address& addr, const Immediate& imm) {
		emit_instr(0xc1, addr, 5);
		emit_immediate(imm, 1);
	}
	
	void Assembler::sub(const Immediate& src, const Register& dst) {
		emit_instr(0x81, dst, 5);
		emit_immediate(src, 4);
	}
	
	void Assembler::sub(const Immediate& src, const Address& dst) {
		emit_instr(0x81, dst, 5);
		emit_immediate(src, 4);
	}
	
	void Assembler::sub(const Register& src, const Register& dst) {
		emit_instr(0x29, src, dst);
	}
	
	void Assembler::sub(const Register& src, const Address& dst) {
		emit_instr(0x29, src, dst);
	}

	void Assembler::sub(const Address& src, const Register& dst) {
		emit_instr(0x2b, dst, src);
	}
	
	void Assembler::test(const Immediate& left, const Register& right) {
		emit_instr(0xf7, right);
		emit_immediate(left, 4);
	}
	
	void Assembler::test(const Register& left, const Register& right) {
		emit_instr(0x85, right, left);
	}
	
	void Assembler::test(const Register& left, const Address& right) {
		emit_instr(0x85, left, right);
	}
	
	void Assembler::test(const Address& left, const Register& right) {
		emit_instr(0x85, right, left);
	}
	
	void Assembler::xchg(const Register& left, const Register& right) {
		emit_instr(0x87, left, right);
	}
	
	void Assembler::xchg(const Register& left, const Address& right) {
		emit_instr(0x87, left, right);
	}
	
	void Assembler::bin_xor(const Immediate& src, const Register& dst) {
		emit_instr(0x81, dst, 6);
		emit_immediate(src);
	}
	
	void Assembler::bin_xor(const Immediate& src, const Address& dst) {
		emit_instr(0x81, dst, 6);
		emit_immediate(src);
	}
	
	void Assembler::bin_xor(const Register& src, const Register& dst) {
		emit_instr(0x31, src, dst);
	}
	
	void Assembler::bin_xor(const Register& src, const Address& dst) {
		emit_instr(0x31, src, dst);
	}

	void Assembler::bin_xor(const Address& src, const Register& dst) {
		emit_instr(0x33, dst, src);
	}
}
}

#endif /* end of include guard: ASSEMBLER_X86_64_CPP_DNNNABGU */

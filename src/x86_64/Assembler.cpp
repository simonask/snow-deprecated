#ifndef ASSEMBLER_X86_64_CPP_DNNNABGU
#define ASSEMBLER_X86_64_CPP_DNNNABGU

#include "x86_64/Assembler.h"

namespace snow {
namespace x86_64 {
	byte Assembler::rex_for_operands(const Register& reg, const Register& rm) {
		int flags = rex_for_operands(0, rm) | REX_WIDE_OPERAND;
		if (reg.extended())
			flags |= REX_EXTEND_REG;
		return flags;
	}
	
	byte Assembler::rex_for_operands(const Register& reg, const Address& rm) {
		int flags = rex_for_operands(reg, rm.reg());
		if (rm.wide())
			flags |= REX_WIDE_OPERAND;
		return flags;
	}
	
	byte Assembler::rex_for_operands(const Register& reg, const SIB& sib) {
		return rex_for_operands(0, reg) | rex_for_operands(0, sib);
	}
	
	byte Assembler::rex_for_operands(int, const Register& rm) {
		int flags = NO_REX;
		if (rm.extended())
			flags |= REX_EXTEND_RM;
		return flags;
	}
	
	byte Assembler::rex_for_operands(int, const Address& rm) {
		int flags = rex_for_operands(0, rm.reg());
		if (rm.wide())
			flags |= REX_WIDE_OPERAND;
		return flags;
	}
	
	byte Assembler::rex_for_operands(int, const SIB& sib) {
		int flags = NO_REX;
		if (sib.base().extended())
			flags |= REX_EXTEND_SIB_BASE;
		if (sib.index().extended())
			flags |= REX_EXTEND_SIB_INDEX;
		if (sib.wide())
			flags |= REX_WIDE_OPERAND;
		return flags;
	}
	
	Assembler::RM_MODE Assembler::mod_for_displacement(int32_t displacement) {
		if (displacement == 0)
			return RM_ADDRESS;
		else if ((int)(char)displacement == displacement)
			return RM_ADDRESS_DISP8;
		else
			return RM_ADDRESS_DISP32;
	}
	
	Assembler::RM_MODE Assembler::mod_for_address(const Address& addr) {
		RM_MODE mod = mod_for_displacement(addr.offset());
		if (mod == RM_ADDRESS && addr.reg() == rbp) {
			return RM_ADDRESS_DISP32;
		}
		return mod;
	}
	
	void Assembler::emit_opcodes(byte* ptr) {
		while (*ptr != (byte)'\x00') {
			emit(*ptr++);
		}
	}
	
	void Assembler::emit_immediate(const Immediate& imm, size_t bytes) {
		int64_t value = imm.data();
		byte* data = reinterpret_cast<byte*>(&value);
		for (size_t i = 0; i < bytes; ++i) {
			emit(data[i]);
		}
	}
	
	void Assembler::emit_sib(const SIB& sib) {
		byte scale = sib.scale() << 6 & 0xc0;
		byte index = sib.index().code() << 3 & 0x38;
		byte base = sib.base().code() & 0x07;
		emit(scale | index | base);
	}
	
	void Assembler::emit_operands(byte mod, byte reg, byte rm) {
		// masking to ensure that nothing spills over
		mod = (mod << 6) & 0xc0;
		reg = (reg << 3) & 0x38;
		rm &= 0x07;
		emit(mod | reg | rm);
	}
	
	void Assembler::emit_operands(byte opcode_ext, const Register& rm) {
		emit_operands(RM_REGISTER, opcode_ext, rm.code());
	}
	
	void Assembler::emit_operands(const Register& reg, const Register& rm) {
		if (rm == rsp)
			error("(x86_64 asm) Cannot use %rsp as target operand.");
		emit_operands(RM_REGISTER, reg.code(), rm.code());
	}
	
	void Assembler::emit_operands(byte opcode_ext, const Address& rm) {
		if (rm.reg() == rsp)
			error("(x86_64 asm) Cannot use %rsp as memory operand.");
		emit_operands(mod_for_address(rm), opcode_ext, rm.reg().code());
		emit_displacement(rm.offset());
	}
	
	void Assembler::emit_operands(const Register& reg, const Address& rm) {
		if (rm.reg() == rsp)
			error("(x86_64 asm) Cannot use %rsp as memory operand.");
		emit_operands(mod_for_address(rm), reg.code(), rm.reg().code());
		emit_displacement(rm.offset());
	}
	
	void Assembler::emit_operands(byte opcode_ext, const SIB& sib) {
		emit_operands(mod_for_displacement(sib.offset()), opcode_ext, 4);
		emit_sib(sib);
		emit_displacement(sib.offset());
	}
	
	void Assembler::emit_operands(const Register& reg, const SIB& sib) {
		emit_operands(mod_for_displacement(sib.offset()), reg.code(), 4);
		emit_sib(sib);
		emit_displacement(sib.offset());
	}
	
	void Assembler::emit_displacement(int32_t displacement) {
		switch (mod_for_displacement(displacement)) {
			case RM_ADDRESS_DISP8:
				emit(displacement);
				break;
			case RM_ADDRESS_DISP32:
				emit_immediate(Immediate(displacement), 4);
			default:
				break;
		}
	}
	
	void Assembler::emit_label_ref(const RefPtr<Label>& label) {
		if (label->bound()) {
			int offset = label->offset() - (this->offset() + 4);
			byte* _offset = reinterpret_cast<byte*>(&offset);
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

	void Assembler::add(const Immediate& src, const Register& dst) {
		emit_instr(0x81, 0, dst, REX_WIDE_OPERAND);
		emit_immediate(src);
	}
	
	void Assembler::add(const Immediate& src, const Address& dst) {
		emit_instr(0x81, 0, dst);
		emit_immediate(src);
	}
	
	void Assembler::add(const Register& src, const Register& dst) {
		emit_instr(0x01, src, dst, REX_WIDE_OPERAND);
	}
	
	void Assembler::add(const Register& src, const Address& dst) {
		emit_instr(0x01, src, dst);
	}

	void Assembler::add(const Address& src, const Register& dst) {
		emit_instr(0x03, dst, src, REX_WIDE_OPERAND);
	}
	
	void Assembler::bin_and(const Immediate& src, const Register& dst) {
		emit_instr(0x81, 4, dst);
		emit_immediate(src);
	}
	
	void Assembler::bin_and(const Immediate& src, const Address& dst) {
		emit_instr(0x81, 4, dst);
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
		emit_rex(rex_for_operands(0, reg));
		emit(0x0f);
		emit(0xc8 + reg.code());
	}
	
	void Assembler::call(const Immediate& rel32off) {
		emit(0xe8);
		emit_immediate(rel32off, 4);
	}
	
	void Assembler::call(const Register& reg) {
		emit_instr(0xff, 2, reg);
	}
	
	void Assembler::call(const Address& addr) {
		emit_instr(0xff, 2, addr);
	}
	
	void Assembler::call(const Symbol& symb) {
		if (symb.external()) {
			mov(Immediate((int64_t)symb.address()), rax);
			call(rax);
		} else {
			call(Immediate(symb.offset() - (offset() + 5)));
		}
	}
	
	void Assembler::call(const std::string& symb, bool absolute) {
		if (!absolute) {
			call(Immediate((int64_t)0));
			m_SymbolReferences.push_back(Linker::Info(symb, offset() - 4, 4, true, -4));
		} else {
			mov(0, rax);
			m_SymbolReferences.push_back(Linker::Info(symb, offset() - 8, 8));
			call(rax);
		}
	}
	
	void Assembler::call_far(const Address& addr) {
		emit_instr(0xff, 3, addr);
	}
	
	void Assembler::cmov(Condition cc, const Register& src, const Register& dst) {
		byte opcodes[2];
		opcodes[0] = 0x0f;
		opcodes[1] = 0x40 + cc;
		emit_instr(opcodes, dst, src);
	}
	
	void Assembler::cmov(Condition cc, const Address& src, const Register& dst) {
		byte opcodes[2];
		opcodes[0] = 0x0f;
		opcodes[1] = 0x40 + cc;
		emit_instr(opcodes, dst, src);
	}
	
	void Assembler::cmp(const Immediate& left, const Register& right, bool single_byte) {
		emit_instr(single_byte ? 0x80 : 0x81, 7, right);
		emit_immediate(left, single_byte ? 1 : 4);
	}
	
	void Assembler::cmp(const Register& left, const Register& right, bool single_byte) {
		emit_instr(single_byte ? 0x38 : 0x39, right, left, REX_WIDE_OPERAND);
	}
	
	void Assembler::cmp(const Register& left, const Address& right, bool single_byte) {
		emit_instr(single_byte ? 0x3a : 0x3b, left, right);
	}
	
	void Assembler::cmp(const Address& left, const Register& right, bool single_byte) {
		emit_instr(single_byte ? 0x38 : 0x39, right, left);
	}
	
	void Assembler::dec(const Register& reg, bool single_byte) {
		emit_instr(single_byte ? 0xfe : 0xff, 1, reg, single_byte ? NO_REX : REX_WIDE_OPERAND);
	}
	
	void Assembler::dec(const Address& addr, bool single_byte) {
		emit_instr(single_byte ? 0xfe : 0xff, 1, addr);
	}
	
	void Assembler::div(const Register& reg) {
		emit_instr(0xf7, 6, reg);
	}
	
	void Assembler::div(const Address& addr) {
		emit_instr(0xf7, 6, addr);
	}
	
	void Assembler::enter(const Immediate& stack_size, const Immediate& nesting_level) {
		emit(0xc8);
		emit_immediate(stack_size, 2);
		emit_immediate(nesting_level, 1);
	}
	
	void Assembler::idiv(const Register& reg) {
		emit_instr(0xf7, 7, reg);
	}
	
	void Assembler::idiv(const Address& addr) {
		emit_instr(0xf7, 7, addr);
	}
	
	void Assembler::imul(const Register& src, const Register& dst) {
		emit_instr((byte*)"\x0f\xaf", dst, src, REX_WIDE_OPERAND);
	}
	
	void Assembler::imul(const Address& src, const Register& dst) {
		emit_instr((byte*)"\x0f\xaf", dst, src);
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
		emit_instr(single_byte ? 0xfe : 0xff, 0, reg, single_byte ? NO_REX : REX_WIDE_OPERAND);
	}
	
	void Assembler::inc(const Address& addr, bool single_byte) {
		emit_instr(single_byte ? 0xfe : 0xff, 0, addr);
	}
	
	void Assembler::j(Condition cc, const RefPtr<Label>& label) {
		emit(0x0f);
		emit(0x80 + cc);
		emit_label_ref(label);
	}
	
	void Assembler::j(Condition cc, const Immediate& rel32off) {
		emit(0x0f);
		emit(0x80 + cc);
		emit_immediate(rel32off, 4);
	}
	
	void Assembler::jmp(const RefPtr<Label>& label) {
		emit(0xe9);
		emit_label_ref(label);
	}
	
	void Assembler::jmp(const Immediate& rel32off) {
		emit(0xe9);
		emit_immediate(rel32off, 4);
	}
	
	void Assembler::jmp(const Register& reg) {
		emit_instr(0xff, 4, reg);
	}
	
	void Assembler::jmp(const Address& addr) {
		emit_instr(0xff, 4, addr);
	}
	
	void Assembler::jmp_far(const Address& addr) {
		emit_instr(0xff, 3, addr);
	}
	
	void Assembler::lea(const Address& src, const Register& dst) {
		emit_instr(0x8d, dst, src);
	}
	
	void Assembler::lzcnt(const Register& src, const Register& dst) {
		emit_instr((byte*)"\xf3\x0f\xbd", dst, src);
	}
	
	void Assembler::lzcnt(const Address& src, const Register& dst) {
		emit_instr((byte*)"\xf3\x0f\xbd", dst, src);
	}
	
	void Assembler::mov(const Register& src, const Register& dst) {
		emit_instr(0x89, src, dst, REX_WIDE_OPERAND);
	}
	
	void Assembler::mov(const Register& src, const Address& dst) {
		emit_instr(0x89, src, dst);
	}
	
	void Assembler::mov(const Register& src, const SIB& dst) {
		emit_instr(0x89, src, dst);
	}
	
	void Assembler::mov(const Address& src, const Register& dst) {
		emit_instr(0x8b, dst, src, REX_WIDE_OPERAND);
	}
	
	void Assembler::mov(const SIB& src, const Register& dst) {
		emit_instr(0x8b, dst, src, REX_WIDE_OPERAND);
	}
	
	void Assembler::mov(const Immediate& src, const Register& dst) {
		emit_rex(rex_for_operands(0, dst) | REX_WIDE_OPERAND);
		emit(0xb8 + dst.code());
		emit_immediate(src, 8);
	}
	
	void Assembler::mov(const Immediate& src, const Address& dst) {
		emit_instr(0xc7, 0, dst);
		emit_immediate(src, 4);
	}
	
	void Assembler::mov(const Immediate& src, const SIB& dst) {
		emit_instr(0xc7, 0, dst);
		emit_immediate(src, 4);
	}
	
	void Assembler::mul(const Register& reg) {
		emit_instr(0xf7, 0, reg, 4);
	}
	
	void Assembler::mul(const Address& addr) {
		emit_instr(0xf7, 0, addr, 4);
	}
	
	void Assembler::neg(const Register& reg) {
		emit_instr(0xf7, 0, reg, 3);
	}
	
	void Assembler::neg(const Address& addr) {
		emit_instr(0xf7, 0, addr, 3);
	}
	
	void Assembler::bin_not(const Register& reg) {
		emit_instr(0xf7, 0, reg, 2);
	}
	
	void Assembler::bin_not(const Address& addr) {
		emit_instr(0xf7, 0, addr, 2);
	}
	
	void Assembler::bin_or(const Immediate& src, const Register& dst) {
		emit_instr(0x81, 0, dst, 1);
		emit_immediate(src);
	}
	
	void Assembler::bin_or(const Immediate& src, const Address& dst) {
		emit_instr(0x81, 0, dst, 1);
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
		emit_rex(rex_for_operands(0, reg));
		emit(0x58 + reg.code());
	}
	
	void Assembler::pop(const Address& addr) {
		emit_instr(0x8f, 0, addr);
	}
	
	void Assembler::popcnt(const Register& src, const Register& dst) {
		emit_instr((byte*)"\xf3\x0f\xb8", dst, src);
	}
	
	void Assembler::popcnt(const Address& src, const Register& dst) {
		emit_instr((byte*)"\xf3\x0f\xb8", dst, src);
	}
	
	void Assembler::push(const Register& reg) {
		emit_rex(rex_for_operands(0, reg));
		emit(0x50 + reg.code());
	}
	
	void Assembler::push(const Immediate& imm) {
		emit(0x68);
		emit_immediate(imm);
	}
	
	void Assembler::push(const Address& addr) {
		emit_instr(0xff, 0, addr, 6);
	}
	
	void Assembler::rol(const Register& reg) {
		emit_instr(0xd1, 0, reg);
	}
	
	void Assembler::rol(const Address& addr) {
		emit_instr(0xd1, 0, addr);
	}
	
	void Assembler::rol(const Register& reg, const Immediate& imm) {
		emit_instr(0xc1, 0, reg);
		emit_immediate(imm, 1);
	}
	
	void Assembler::rol(const Address& addr, const Immediate& imm) {
		emit_instr(0xc1, 0, addr);
		emit_immediate(imm, 1);
	}
	
	void Assembler::ror(const Register& reg) {
		emit_instr(0xd1, 0, reg);
	}
	
	void Assembler::ror(const Address& addr) {
		emit_instr(0xd1, 0, addr);
	}
	
	void Assembler::ror(const Register& reg, const Immediate& imm) {
		emit_instr(0xc1, 1, reg);
		emit_immediate(imm, 1);
	}
	
	void Assembler::ror(const Address& addr, const Immediate& imm) {
		emit_instr(0xc1, 1, addr);
		emit_immediate(imm, 1);
	}
	
	void Assembler::shl(const Register& reg) {
		emit_instr(0xd1, 4, reg);
	}
	
	void Assembler::shl(const Address& addr) {
		emit_instr(0xf1, 4, addr);
	}
	
	void Assembler::shl(const Register& reg, const Immediate& imm) {
		emit_instr(0xc1, 4, reg);
		emit_immediate(imm, 1);
	}
	
	void Assembler::shl(const Address& addr, const Immediate& imm) {
		emit_instr(0xc1, 5, addr);
		emit_immediate(imm, 1);
	}
	
	void Assembler::shr(const Register& reg) {
		emit_instr(0xd1, 5, reg);
	}
	
	void Assembler::shr(const Address& addr) {
		emit_instr(0xd1, 5, addr);
	}
	
	void Assembler::shr(const Register& reg, const Immediate& imm) {
		emit_instr(0xc1, 5, reg);
		emit_immediate(imm, 1);
	}
	
	void Assembler::shr(const Address& addr, const Immediate& imm) {
		emit_instr(0xc1, 5, addr);
		emit_immediate(imm, 1);
	}
	
	void Assembler::sub(const Immediate& src, const Register& dst) {
		emit_instr(0x81, 5, dst, REX_WIDE_OPERAND);
		emit_immediate(src, 4);
	}
	
	void Assembler::sub(const Immediate& src, const Address& dst) {
		emit_instr(0x81, 5, dst);
		emit_immediate(src, 4);
	}
	
	void Assembler::sub(const Register& src, const Register& dst) {
		emit_instr(0x29, src, dst, REX_WIDE_OPERAND);
	}
	
	void Assembler::sub(const Register& src, const Address& dst) {
		emit_instr(0x29, src, dst);
	}

	void Assembler::sub(const Address& src, const Register& dst) {
		emit_instr(0x2b, dst, src);
	}
	
	void Assembler::test(const Immediate& left, const Register& right) {
		emit_instr(0xf7, 0, right);
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
		emit_instr(0x81, 6, dst);
		emit_immediate(src);
	}
	
	void Assembler::bin_xor(const Immediate& src, const Address& dst) {
		emit_instr(0x81, 6, dst);
		emit_immediate(src);
	}
	
	void Assembler::bin_xor(const Register& src, const Register& dst) {
		emit_instr(0x31, src, dst, REX_WIDE_OPERAND);
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

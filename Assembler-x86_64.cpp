#ifndef ASSEMBLER_X86_64_CPP_DNNNABGU
#define ASSEMBLER_X86_64_CPP_DNNNABGU

#include "Assembler-x86_64.h"

namespace x86_64 {
	void Assembler::emit(char code) {
		m_Code.append(code);
	}
	
	unsigned char Assembler::rex_for_operands(const Register& reg, const Register& rm) {
		int flags = rex_for_operand(rm);
		if (reg.extended())
			flags |= REX_EXTEND_REG;
		return flags;
	}
	
	unsigned char Assembler::rex_for_operand(const Register& rm_or_opcode_reg) {
		int flags = NO_REX;
		if (rm_or_opcode_reg.extended())
			flags |= REX_EXTEND_RM;
		return REX_WIDE_OPERAND;
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
			int offset = label.offset() - (m_Code.length() + 4);
			unsigned char* _offset = reinterpret_cast<unsigned char*>(&offset);
			for (int i = 0; i < 4; ++i) {
				emit(_offset[i]);
			}
		} else {
			m_UnboundLabelReferences.push_back(UnboundLabelReference(label, m_Code.length(), 4));
			for (int i = 0; i < 4; ++i) {
				emit(0x00);
			}
		}
	}
	
	Assembler::RM_MODE Assembler::mod_for_address(const Address& addr) {
		if (addr.offset() == 0)
			return RM_ADDRESS;
		else if (addr.offset() & 0xffffff00 != 0)
			return RM_ADDRESS_DISP32;
		else
			return RM_ADDRESS_DISP8;
	}
	
	void Assembler::bind(Label& label) {
		label.bind(m_Code.length());
		
		std::vector<UnboundLabelReference>::iterator iter;
		for (iter = m_UnboundLabelReferences.begin(); iter != m_UnboundLabelReferences.end(); ++iter) {
			if (iter->label == &label) {
				int offset = label.offset() - (iter->offset + 4);
				unsigned char* _offset = reinterpret_cast<unsigned char*>(&offset);
				for (int i = 0; i < iter->size; ++i) {
					m_Code[iter->offset + i] = _offset[i];
				}
				
				//iter = m_UnboundLabelReferences.erase(iter);
			}
		}
	}
	
	void Assembler::add(const Immediate& src, const Register& dst) {
		emit_rex(rex_for_operand(dst));
		emit(0x81);
		emit_modrm(dst);
		emit_immediate(src);
	}
	
	void Assembler::add(const Immediate& src, const Address& dst) {
		emit_rex(rex_for_operand(dst.reg()));
		emit(0x81);
		emit_modrm(dst);
		emit_immediate(src);
	}
	
	void Assembler::add(const Register& src, const Register& dst) {
		emit_rex(rex_for_operands(src, dst));
		emit(0x01);
		emit_modrm(src, dst);
	}
	
	void Assembler::add(const Register& src, const Address& dst) {
		emit_rex(rex_for_operands(src, dst.reg()));
		emit(0x01);
		emit_modrm(src, dst);
	}

	void Assembler::add(const Address& src, const Register& dst) {
		emit_rex(rex_for_operands(dst, src.reg()));
		emit(0x03);
		emit_modrm(dst, src);
	}
	
	void Assembler::bin_and(const Immediate& src, const Register& dst) {
		emit_rex(rex_for_operand(dst));
		emit(0x81);
		emit_modrm(dst, 4);
		emit_immediate(src);
	}
	
	void Assembler::bin_and(const Immediate& src, const Address& dst) {
		emit_rex(rex_for_operand(dst.reg()));
		emit(0x81);
		emit_modrm(dst, 4);
		emit_immediate(src);
	}
	
	void Assembler::bin_and(const Register& src, const Register& dst) {
		emit_rex(rex_for_operands(src, dst));
		emit(0x21);
		emit_modrm(src, dst);
	}
	
	void Assembler::bin_and(const Register& src, const Address& dst) {
		emit_rex(rex_for_operands(src, dst.reg()));
		emit(0x21);
		emit_modrm(src, dst);
	}

	void Assembler::bin_and(const Address& src, const Register& dst) {
		emit_rex(rex_for_operands(dst, src.reg()));
		emit(0x23);
		emit_modrm(dst, src);
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
		emit_rex(rex_for_operand(reg));
		emit(0xff);
		emit_modrm(reg, 2);
	}
	
	void Assembler::call(const Address& addr) {
		emit_rex(rex_for_operand(addr));
		emit(0xff);
		emit_modrm(addr, 2);
	}
	
	void Assembler::call_far(const Address& addr) {
		emit_rex(rex_for_operand(addr));
		emit(0xff);
		emit_modrm(addr, 3);
	}
	
	void Assembler::cmov(Condition cc, const Register& src, const Register& dst) {
		emit_rex(rex_for_operands(src, dst));
		emit(0x0f);
		emit(0x40 + cc);
		emit_modrm(dst, src);
	}
	
	void Assembler::cmov(Condition cc, const Address& src, const Register& dst) {
		emit_rex(rex_for_operands(src.reg(), dst));
		emit(0x0f);
		emit(0x40 + cc);
		emit_modrm(dst, src);
	}
	
	void Assembler::cmp(const Immediate& left, const Register& right) {
		emit_rex(rex_for_operand(right));
		emit(0x81);
		emit_modrm(right, 7);
		emit_immediate(left);
	}
	
	void Assembler::cmp(const Register& left, const Register& right) {
		emit_rex(rex_for_operands(right, left));
		emit(0x39);
		emit_modrm(right, left);
	}
	
	void Assembler::cmp(const Register& left, const Address& right) {
		emit_rex(rex_for_operands(left, right.reg()));
		emit(0x3b);
		emit_modrm(left, right);
	}
	
	void Assembler::cmp(const Address& left, const Register& right) {
		emit_rex(rex_for_operands(right, left));
		emit(0x39);
		emit_modrm(right, left);
	}
	
	void Assembler::dec(const Register& reg) {
		emit_rex(rex_for_operand(reg));
		emit(0xff);
		emit_modrm(reg, 1);
	}
	
	void Assembler::dec(const Address& addr) {
		emit_rex(rex_for_operand(addr));
		emit(0xff);
		emit_modrm(addr, 1);
	}
	
	void Assembler::div(const Register& reg) {
		emit_rex(rex_for_operand(reg));
		emit(0xf7);
		emit_modrm(reg, 6);
	}
	
	void Assembler::div(const Address& addr) {
		emit_rex(rex_for_operand(addr));
		emit(0xf7);
		emit_modrm(addr, 6);
	}
	
	void Assembler::enter(const Immediate& stack_size, const Immediate& nesting_level) {
		emit(0xc8);
		emit_immediate(stack_size, 2);
		emit_immediate(nesting_level, 1);
	}
	
	void Assembler::idiv(const Register& reg) {
		emit_rex(rex_for_operand(reg));
		emit(0xf7);
		emit_modrm(reg, 7);
	}
	
	void Assembler::idiv(const Address& addr) {
		emit_rex(rex_for_operand(addr));
		emit(0xf7);
		emit_modrm(addr, 7);
	}
	
	void Assembler::imul(const Register& src, const Register& dst) {
		emit_rex(rex_for_operands(dst, src));
		emit(0x0f);
		emit(0xaf);
		emit_modrm(dst, src);
	}
	
	void Assembler::imul(const Address& src, const Register& dst) {
		emit_rex(rex_for_operands(dst, src));
		emit(0x0f);
		emit(0xaf);
		emit_modrm(dst, src);
	}
	
	void Assembler::imul(const Immediate& imm, const Register& src, const Register& dst) {
		emit_rex(rex_for_operands(dst, src));
		emit(0x69);
		emit_modrm(dst, src);
		emit_immediate(imm, 4);
	}
	
	void Assembler::imul(const Immediate& imm, const Address& src, const Register& dst) {
		emit_rex(rex_for_operands(dst, src));
		emit(0x69);
		emit_modrm(dst, src);
		emit_immediate(imm, 4);
	}
	
	void Assembler::inc(const Register& reg) {
		emit_rex(rex_for_operand(reg));
		emit(0xff);
		emit_modrm(reg);
	}
	
	void Assembler::inc(const Address& addr) {
		emit_rex(rex_for_operand(addr));
		emit(0xff);
		emit_modrm(addr);
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
		emit_rex(rex_for_operand(reg));
		emit(0xff);
		emit_modrm(reg, 4);
	}
	
	void Assembler::jmp(const Address& addr) {
		emit_rex(rex_for_operand(addr));
		emit(0xff);
		emit_modrm(addr, 4);
	}
	
	void Assembler::jmp_far(const Address& addr) {
		emit_rex(rex_for_operand(addr));
		emit(0xff);
		emit_modrm(addr, 5);
	}
	
	void Assembler::lea(const Address& src, const Register& dst) {
		emit_rex(rex_for_operands(dst, src));
		emit(0x8d);
		emit_modrm(dst, src);
	}
	
	void Assembler::lzcnt(const Register& src, const Register& dst) {
		emit_rex(rex_for_operands(dst, src));
		emit(0xf3);
		emit(0x0f);
		emit(0xbd);
		emit_modrm(dst, src);
	}
	
	void Assembler::lzcnt(const Address& src, const Register& dst) {
		emit_rex(rex_for_operands(dst, src));
		emit(0xf3);
		emit(0x0f);
		emit(0xbd);
		emit_modrm(dst, src);
	}
	
	void Assembler::mov(const Register& src, const Register& dst) {
		emit_rex(rex_for_operands(src, dst) | REX_WIDE_OPERAND);
		emit(0x89);
		emit_modrm(src, dst);
	}
	
	void Assembler::mov(const Register& src, const Address& dst) {
		emit_rex(rex_for_operands(src, dst.reg()) | REX_WIDE_OPERAND);
		emit(0x89);
		emit_modrm(src, dst);
	}
	
	void Assembler::mov(const Address& src, const Register& dst) {
		emit_rex(rex_for_operands(dst, src.reg()) | REX_WIDE_OPERAND);
		emit(0x8b);
		emit_modrm(dst, src);
	}
	
	void Assembler::mov(const Immediate& src, const Register& dst) {
		emit_rex(rex_for_operand(dst) | REX_WIDE_OPERAND);
		emit(0xb8 + dst.code());
		emit_immediate(src, 8);
	}
	
	void Assembler::mov(const Immediate& src, const Address& dst) {
		emit_rex(rex_for_operand(dst));
		emit(0xc7);
		emit_modrm(dst);
		emit_immediate(src, 4);
	}
	
	void Assembler::mul(const Register& reg) {
		emit_rex(rex_for_operand(reg));
		emit(0xf7);
		emit_modrm(reg, 4);
	}
	
	void Assembler::mul(const Address& addr) {
		emit_rex(rex_for_operand(addr));
		emit(0xf7);
		emit_modrm(addr, 4);
	}
	
	void Assembler::neg(const Register& reg) {
		emit_rex(rex_for_operand(reg));
		emit(0xf7);
		emit_modrm(reg, 3);
	}
	
	void Assembler::neg(const Address& addr) {
		emit_rex(rex_for_operand(addr));
		emit(0xf7);
		emit_modrm(addr, 3);
	}
	
	void Assembler::bin_not(const Register& reg) {
		emit_rex(rex_for_operand(reg));
		emit(0xf7);
		emit_modrm(reg, 2);
	}
	
	void Assembler::bin_not(const Address& addr) {
		emit_rex(rex_for_operand(addr));
		emit(0xf7);
		emit_modrm(addr, 2);
	}
	
	void Assembler::bin_or(const Immediate& src, const Register& dst) {
		emit_rex(rex_for_operand(dst));
		emit(0x81);
		emit_modrm(dst, 1);
		emit_immediate(src);
	}
	
	void Assembler::bin_or(const Immediate& src, const Address& dst) {
		emit_rex(rex_for_operand(dst));
		emit(0x81);
		emit_modrm(dst, 1);
		emit_immediate(src);
	}
	
	void Assembler::bin_or(const Register& src, const Register& dst) {
		emit_rex(rex_for_operands(src, dst));
		emit(0x09);
		emit_modrm(src, dst);
	}
	
	void Assembler::bin_or(const Register& src, const Address& dst) {
		emit_rex(rex_for_operands(src, dst));
		emit(0x09);
		emit_modrm(src, dst);
	}

	void Assembler::bin_or(const Address& src, const Register& dst) {
		emit_rex(rex_for_operands(dst, src));
		emit(0x0a);
		emit_modrm(dst, src);
	}
	
	void Assembler::pop(const Register& reg) {
		emit_rex(rex_for_operand(reg));
		emit(0x58 + reg.code());
	}
	
	void Assembler::pop(const Address& addr) {
		emit_rex(rex_for_operand(addr.reg()));
		emit(0x8f);
		emit_modrm(addr);
	}
	
	void Assembler::popcnt(const Register& src, const Register& dst) {
		emit_rex(rex_for_operands(dst, src));
		emit(0xf3);
		emit(0x0f);
		emit(0xb8);
		emit_modrm(dst, src);
	}
	
	void Assembler::popcnt(const Address& src, const Register& dst) {
		emit_rex(rex_for_operands(dst, src));
		emit(0xf3);
		emit(0x0f);
		emit(0xb8);
		emit_modrm(dst, src);
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
		emit_rex(rex_for_operand(addr.reg()));
		emit(0xff); // push
		emit_modrm(rsi, addr);
	}
	
	void Assembler::rol(const Register& reg) {
		emit_rex(rex_for_operand(reg));
		emit(0xd1);
		emit_modrm(reg);
	}
	
	void Assembler::rol(const Address& addr) {
		emit_rex(rex_for_operand(addr));
		emit(0xd1);
		emit_modrm(addr);
	}
	
	void Assembler::rol(const Register& reg, const Immediate& imm) {
		emit_rex(rex_for_operand(reg));
		emit(0xc1);
		emit_modrm(reg);
		emit_immediate(imm, 1);
	}
	
	void Assembler::rol(const Address& addr, const Immediate& imm) {
		emit_rex(rex_for_operand(addr));
		emit(0xc1);
		emit_modrm(addr);
		emit_immediate(imm, 1);
	}
	
	void Assembler::ror(const Register& reg) {
		emit_rex(rex_for_operand(reg));
		emit(0xd1);
		emit_modrm(reg, 1);
	}
	
	void Assembler::ror(const Address& addr) {
		emit_rex(rex_for_operand(addr));
		emit(0xd1);
		emit_modrm(addr, 1);
	}
	
	void Assembler::ror(const Register& reg, const Immediate& imm) {
		emit_rex(rex_for_operand(reg));
		emit(0xc1);
		emit_modrm(reg, 1);
		emit_immediate(imm, 1);
	}
	
	void Assembler::ror(const Address& addr, const Immediate& imm) {
		emit_rex(rex_for_operand(addr));
		emit(0xc1);
		emit_modrm(addr, 1);
		emit_immediate(imm, 1);
	}
	
	void Assembler::shl(const Register& reg) {
		emit_rex(rex_for_operand(reg));
		emit(0xd1);
		emit_modrm(reg, 4);
	}
	
	void Assembler::shl(const Address& addr) {
		emit_rex(rex_for_operand(addr));
		emit(0xd1);
		emit_modrm(addr, 4);
	}
	
	void Assembler::shl(const Register& reg, const Immediate& imm) {
		emit_rex(rex_for_operand(reg));
		emit(0xc1);
		emit_modrm(reg, 4);
		emit_immediate(imm, 1);
	}
	
	void Assembler::shl(const Address& addr, const Immediate& imm) {
		emit_rex(rex_for_operand(addr));
		emit(0xc1);
		emit_modrm(addr, 4);
		emit_immediate(imm, 1);
	}
	
	void Assembler::shr(const Register& reg) {
		emit_rex(rex_for_operand(reg));
		emit(0xd1);
		emit_modrm(reg, 5);
	}
	
	void Assembler::shr(const Address& addr) {
		emit_rex(rex_for_operand(addr));
		emit(0xd1);
		emit_modrm(addr, 5);
	}
	
	void Assembler::shr(const Register& reg, const Immediate& imm) {
		emit_rex(rex_for_operand(reg));
		emit(0xc1);
		emit_modrm(reg, 5);
		emit_immediate(imm, 1);
	}
	
	void Assembler::shr(const Address& addr, const Immediate& imm) {
		emit_rex(rex_for_operand(addr));
		emit(0xc1);
		emit_modrm(addr, 5);
		emit_immediate(imm, 1);
	}
	
	void Assembler::sub(const Immediate& src, const Register& dst) {
		emit_rex(rex_for_operand(dst));
		emit(0x81);
		emit_modrm(dst, 5);
		emit_immediate(src, 4);
	}
	
	void Assembler::sub(const Immediate& src, const Address& dst) {
		emit_rex(rex_for_operand(dst.reg()));
		emit(0x81);
		emit_modrm(dst, 5);
		emit_immediate(src, 4);
	}
	
	void Assembler::sub(const Register& src, const Register& dst) {
		emit_rex(rex_for_operands(src, dst));
		emit(0x29);
		emit_modrm(src, dst);
	}
	
	void Assembler::sub(const Register& src, const Address& dst) {
		emit_rex(rex_for_operands(src, dst.reg()));
		emit(0x29);
		emit_modrm(src, dst);
	}

	void Assembler::sub(const Address& src, const Register& dst) {
		emit_rex(rex_for_operands(dst, src.reg()));
		emit(0x2b);
		emit_modrm(dst, src);
	}
	
	void Assembler::test(const Immediate& left, const Register& right) {
		emit_rex(rex_for_operand(right));
		emit(0xf7);
		emit_modrm(right);
		emit_immediate(left, 4);
	}
	
	void Assembler::test(const Register& left, const Register& right) {
		emit_rex(rex_for_operands(right, left));
		emit(0x85);
		emit_modrm(right, left);
	}
	
	void Assembler::test(const Register& left, const Address& right) {
		emit_rex(rex_for_operands(left, right.reg()));
		emit(0x85);
		emit_modrm(left, right);
	}
	
	void Assembler::test(const Address& left, const Register& right) {
		emit_rex(rex_for_operands(right, left));
		emit(0x85);
		emit_modrm(right, left);
	}
	
	void Assembler::xchg(const Register& left, const Register& right) {
		emit_rex(rex_for_operands(left, right));
		emit(0x87);
		emit_modrm(left, right);
	}
	
	void Assembler::xchg(const Register& left, const Address& right) {
		emit_rex(rex_for_operands(left, right));
		emit(0x87);
		emit_modrm(left, right);
	}
	
	void Assembler::bin_xor(const Immediate& src, const Register& dst) {
		emit_rex(rex_for_operand(dst));
		emit(0x81);
		emit_modrm(dst, 6);
		emit_immediate(src);
	}
	
	void Assembler::bin_xor(const Immediate& src, const Address& dst) {
		emit_rex(rex_for_operand(dst));
		emit(0x81);
		emit_modrm(dst, 6);
		emit_immediate(src);
	}
	
	void Assembler::bin_xor(const Register& src, const Register& dst) {
		emit_rex(rex_for_operands(src, dst));
		emit(0x31);
		emit_modrm(src, dst);
	}
	
	void Assembler::bin_xor(const Register& src, const Address& dst) {
		emit_rex(rex_for_operands(src, dst));
		emit(0x31);
		emit_modrm(src, dst);
	}

	void Assembler::bin_xor(const Address& src, const Register& dst) {
		emit_rex(rex_for_operands(dst, src));
		emit(0x33);
		emit_modrm(dst, src);
	}
};

#endif /* end of include guard: ASSEMBLER_X86_64_CPP_DNNNABGU */

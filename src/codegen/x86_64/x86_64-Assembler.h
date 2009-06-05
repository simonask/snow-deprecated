#ifndef ASSEMBER_X86_64_H_OO5SJZ17
#define ASSEMBER_X86_64_H_OO5SJZ17

#include "codegen/Assembler.h"
#include "x86_64-Operand.h"
#include "codegen/Label.h"
#include "base/Basic.h"
#include <vector>
#include <string>

namespace snow {
namespace x86_64 {
	enum Condition {
		CC_OVERFLOW = 0,
		CC_NOT_OVERFLOW = 1,
		CC_BELOW = 2,
		CC_CARRY = 2,
		CC_NOT_BELOW = 3,
		CC_NOT_CARRY = 3,
		CC_ZERO = 4,
		CC_EQUAL = 4,
		CC_NOT_ZERO = 5,
		CC_NOT_EQUAL = 5,
		CC_NOT_ABOVE = 6,
		CC_ABOVE = 7,
		CC_SIGN = 8,
		CC_NOT_SIGN = 9,
		CC_PARITY_EVEN = 0xA,
		CC_PARITY_ODD = 0xB,
		CC_LESS = 0xC,
		CC_GREATER_EQUAL = 0xD,
		CC_LESS_EQUAL = 0xE,
		CC_GREATER = 0xF
	};
	
	class Assembler : public snow::Assembler {
	public:
		enum RM_MODE { // the "mod" part of the modrm operand byte
			RM_ADDRESS = 0,
			RM_ADDRESS_DISP8 = 1,
			RM_ADDRESS_DISP32 = 2,
			RM_REGISTER = 3
		};
		enum REX_FLAGS {
			NO_REX = 0,
			REX_EXTEND_RM = 1,
			REX_EXTEND_OPCODE = 1,
			REX_EXTEND_SIB_BASE = 1,
			REX_EXTEND_SIB_INDEX = 1 << 1,
			REX_EXTEND_REG = 1 << 2, // REX.R
			REX_WIDE_OPERAND = 1 << 3 // REX.B
		};
		
	private:
		void emit_opcodes(const byte* opcodes);
		void emit_rex(int rex_flags) { if (rex_flags != NO_REX) emit(0x40 | rex_flags); }
		byte rex_for_operands(const Register& reg, const Address& rm);
		byte rex_for_operands(const Register& reg, const Register& rm);
		byte rex_for_operands(const Register& reg, const SIB& sib);
		byte rex_for_operands(int, const Address& rm);
		byte rex_for_operands(int, const Register& rm);
		byte rex_for_operands(int, const SIB& sib);
		void emit_immediate(const Immediate&, size_t bytes = 4);
		RM_MODE mod_for_displacement(int32_t displacement);
		RM_MODE mod_for_address(const Address& addr);
		void emit_displacement(int32_t displacement);
		void emit_label_ref(const RefPtr<Label>& label);
		
		void emit_sib(const SIB& sib);
		void emit_operands(byte mod, byte reg, byte rm);
		
		void emit_operands(byte opcode_ext, const Register& rm);
		void emit_operands(const Register& reg, const Register& rm);
		void emit_operands(byte opcode_ext, const Address& rm);
		void emit_operands(const Register& reg, const Address& rm);
		void emit_operands(byte opcode_ext, const SIB& sib);
		void emit_operands(const Register& reg, const SIB& sib);
		
		/*
			Most instructions take the form of: REX | OPCODE | OPERANDS(MODRM+SIB)
			These cases are handled by emit_instr. Other instructions need to
			use emit() and friends directly.
		*/
		template <class regT, class rmT>
		void emit_instr(const byte* opcodes, const regT& reg, const rmT& rm, int extra_rex = NO_REX);
		template <typename regT, class rmT>
		void emit_instr(byte opcode, const regT& reg, const rmT& rm, int extra_rex = NO_REX);
	public:
		~Assembler() {} // not polymorphic
		
		void add(const Immediate& src, const Register& dst);
		void add(const Immediate& src, const Address& dst);
		void add(const Register& src, const Register& dst);
		void add(const Register& src, const Address& dst);
		void add(const Address& src, const Register& dst);
		
		void bin_and(const Immediate& src, const Register& dst);
		void bin_and(const Immediate& src, const Address& dst);
		void bin_and(const Register& src, const Register& dst);
		void bin_and(const Register& src, const Address& dst);
		void bin_and(const Address& src, const Register& dst);
		
		void bswap(const Register& reg);
		
		void call(const Immediate& rel32off);
		void call(const Register& reg);
		void call(const Address& addr);
		
		/*
			Call symbol, with immediate inline linking.
			Whether the Linker::Symbol is external or internal decides if the generated
			code will be position-dependent or not.
			
			External symbols will generate position-independent code.
			
			Calling internal symbols across subassemblers is undefined.
			Use call(const std::string&, bool absolute = false) if you want to
			ensure correct linking.
		*/
		void call(const Linker::Symbol& symb);
		
		/*
			Call named symbol, with later linking. (see Linker)
			The "absolute" argument indicates if the generated code should
			be position-independent or not. Default is position-dependent
			(relative) addressing, since this is generally faster on amd64,
			and easier to debug.
		*/
		void call(const std::string& symb);
		void call(const char* symb) { call(std::string(symb)); }
		void call_far(const Address& addr);
		
		// Alias for bin_xor(reg, reg).
		void clear(const Register& reg) { bin_xor(reg, reg); }
		
		void cmov(Condition cc, const Register& src, const Register& dst);
		void cmov(Condition cc, const Address& src, const Register& dst);
		
		void cmp(const Immediate& left, const Register& right, bool single_byte = false);
		void cmp(const Register& left, const Register& right, bool single_byte = false);
		void cmp(const Register& left, const Address& right, bool single_byte = false);
		void cmp(const Address& left, const Register& right, bool single_byte = false);
		void cmpb(const Immediate& left, const Register& right) { cmp(left, right, true); }
		void cmpb(const Register& left, const Register& right) { cmp(left, right, true); }
		void cmpb(const Register& left, const Address& right) { cmp(left, right, true); }
		void cmpb(const Address& left, const Register& right) { cmp(left, right, true); }
		
		void cpuid() { emit(0x0f); emit(0xa2); }
		
		void dec(const Register&, bool single_byte = false);
		void dec(const Address&, bool single_byte = false);
		void decb(const Register& reg) { dec(reg, true); }
		void decb(const Address& addr) { dec(addr, true); }
		
		void div(const Register&);
		void div(const Address&);
		
		void enter(const Immediate& stack_size, const Immediate& nesting_level);
		void enter(const Immediate& stack_size) { enter(stack_size, 0); }
		void enter() { enter(0); }
		
		void idiv(const Register&);
		void idiv(const Address&);
		
		void imul(const Register& src, const Register& dst);
		void imul(const Address& src, const Register& dst);
		void imul(const Immediate& imm, const Register& src, const Register& dst);
		void imul(const Immediate& imm, const Address& src, const Register& dst);
		
		void inc(const Register&, bool single_byte = false);
		void inc(const Address&, bool single_byte = false);
		void incb(const Register& reg) { inc(reg, true); }
		void incb(const Address& addr) { inc(addr, true); }
		
		void interrupt(const Immediate& imm) { emit(0xcd); emit_immediate(imm, 1); }
		void int3() { emit(0xcc); }
		// Alias for int3
		void debug_break() { int3(); }
		
		void j(Condition cc, const RefPtr<Label>& label);
		void j(Condition cc, const Immediate& rel32off);
		void jmp(const RefPtr<Label>& label);
		void jmp(const Immediate& rel32off);
		void jmp(const Register& reg);
		void jmp(const Address& addr);
		
		void jmp_far(const Address& addr);
		
		void lahf() { emit(0x9f); }
		
		void lea(const Address& src, const Register& dst);
		
		void leave() { emit(0xc9); }
		
		void lfence() { emit(0x0f); emit(0xae); emit(0xe8); }
		
		void lzcnt(const Register& src, const Register& dst);
		void lzcnt(const Address& src, const Register& dst);
		
		void mfence() { emit(0x0f); emit(0xae); emit(0xf0); }
		
		void mov(const Register& src, const Register& dst);
		void mov(const Address& src, const Register& dst);
		void mov(const SIB& src, const Register& dst);
		void mov(const Register& src, const Address& dst);
		void mov(const Register& src, const SIB& sib);
		void mov(const Immediate& src, const Register& dst);
		void mov(const Immediate& src, const Address& dst);
		void mov(const Immediate& src, const SIB& dst);

		void movd_gpr_xmm(const Register& src, const Register& dst);
		void movd_xmm_gpr(const Register& src, const Register& dst);
		void movd(const Address& src, const Register& dst);
		void movd(const SIB& src, const Register& dst);
		void movd(const Register& src, const Address& dst);
		void movd(const Register& src, const SIB& dst);
		
		void mul(const Register& reg);
		void mul(const Address& addr);
		
		void neg(const Register& reg);
		void neg(const Address& addr);
		
		void nop() { emit(0x90); }
		
		void bin_not(const Register& reg);
		void bin_not(const Address& addr);
		
		void bin_or(const Immediate& src, const Register& dst);
		void bin_or(const Immediate& src, const Address& dst);
		void bin_or(const Register& src, const Register& dst);
		void bin_or(const Register& src, const Address& dst);
		void bin_or(const Address& src, const Register& dst);
		
		void pause() { emit(0xf3); emit(0x90); }
		
		void pop(const Register&);
		void pop(const Address&);
		
		void popcnt(const Register& src, const Register& dst);
		void popcnt(const Address& src, const Register& dst);
		
		void popf() { emit(0x9d); }
		
		void push(const Register&);
		void push(const Immediate&);
		void push(const Address&);
		
		void pushf() { emit(0x9c); }

		void ret() { emit(0xc3); }
		void ret_far() { emit(0xcb); }
		
		void sahf() { emit(0x9e); }
		
		void stc() { emit(0xf9); }
		void std() { emit(0xfd); }
		
		void sub(const Immediate& src, const Register& dst);
		void sub(const Immediate& src, const Address& dst);
		void sub(const Register& src, const Register& dst);
		void sub(const Register& src, const Address& dst);
		void sub(const Address& src, const Register& dst);
		
		void test(const Immediate& left, const Register& right);
		void test(const Register& left, const Register& right);
		void test(const Register& left, const Address& right);
		void test(const Address& left, const Register& right);
		
		void xchg(const Register& left, const Register& right);
		void xchg(const Register& left, const Address& right);
		void xchg(const Address& left, const Register& right) { xchg(right, left); }
		
		void bin_xor(const Immediate& src, const Register& dst);
		void bin_xor(const Immediate& src, const Address& dst);
		void bin_xor(const Register& src, const Register& dst);
		void bin_xor(const Register& src, const Address& dst);
		void bin_xor(const Address& src, const Register& dst);
	};
}
}

#endif /* end of include guard: ASSEMBER_X86_64_H_OO5SJZ17 */

#ifndef ASSEMBER_X86_64_H_OO5SJZ17
#define ASSEMBER_X86_64_H_OO5SJZ17

#include "Operand-x86_64.h"
#include <vector>
#include <string>
#include <tr1/unordered_map>

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
	
	class Assembler {
	private:
		std::vector<unsigned char> m_Code;
	public:
		class Label {
		friend class Assembler;
		private:
			bool m_Bound;
			int m_Offset;
			
			void bind(int offs) { m_Offset = offs; m_Bound = true; }
		public:
			Label() : m_Bound(false), m_Offset(0) {}
			bool bound() const { return m_Bound; }
			int offset() const { return m_Offset; }
			
			bool operator==(const Label& other) const { return m_Bound == other.m_Bound && m_Offset == other.m_Offset; }
		};
		
		class Symbol {
		private:
			bool m_External;
			union {
				int32_t m_Offset;
				void* m_Address;
			};
		public:
			Symbol() {}
			explicit Symbol(int32_t offset) : m_External(false) { m_Offset = offset; }
			explicit Symbol(void* address) : m_External(true) { m_Address = address; }
			bool external() const { return m_External; }
			int32_t offset() const { return m_Offset; }
			void* address() const { return m_Address; }
		};
		
		typedef std::tr1::unordered_map<std::string, Symbol> SymbolTable;
	protected:
		static Symbol define_symbol(SymbolTable& table, const std::string& name, const Symbol& symb);
		
		SymbolTable m_InternalSymbols;
		
		struct UnboundLabelReference {
			const Label* label;
			int offset;
			int size;
			bool absolute;
			
			UnboundLabelReference(const Label& l, int offs, int sz = 4, bool abs = false) : label(&l), offset(offs), size(sz), absolute(abs) {}
		};
		std::vector<UnboundLabelReference> m_UnboundLabelReferences;
		
		struct UnboundExternalSymbolReference {
			std::string name;
			int offset;
			int size;
			
			UnboundExternalSymbolReference(const std::string n, int offs, int sz = 4) : name(n), offset(offs), size(sz) {}
		};
		std::vector<UnboundExternalSymbolReference> m_UnboundExternalSymbolReferences;
		
		enum RM_MODE {
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
		
		void emit(unsigned char code);
		void emit_rex(int rex_flags) { if (rex_flags != NO_REX) emit(0x40 | rex_flags); }
		unsigned char rex_for_operands(const Register& reg, const Register& rm);
		unsigned char rex_for_operand(const Register& rm_or_opcode_register);
		void emit_immediate(const Immediate&, size_t bytes = 4);
		RM_MODE mod_for_address(const Address& addr);
		void emit_displacement(const Address& addr);
		void emit_label_ref(const Label& label);
		
		void emit_modrm(unsigned char mod, unsigned char reg, unsigned char rm);
		void emit_modrm(const Register& rm, unsigned char opcode_ext = 0);
		void emit_modrm(const Register& reg, const Register& rm);
		void emit_modrm(const Address& rm, unsigned char opcode_ext = 0);
		void emit_modrm(const Register& reg, const Address& rm);
	public:
		Assembler() {}
		~Assembler() {}
		
		void bind(Label& label);
		//ByteString code() { return m_Code; }
		size_t pc_offset() const { return m_Code.size(); }
		size_t length() const { return m_Code.size(); }
		void compile_to(unsigned char* buffer, SymbolTable& table) const;
		
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
		void call(const Symbol& symb);
		void call(const std::string& symb);
		void call_far(const Address& addr);
		Symbol define_symbol(const std::string& name);
		
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
		void enter(const Immediate& stack_size) { enter(stack_size, Immediate(0)); }
		
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
		
		void j(Condition cc, const Label& label);
		void j(Condition cc, const Immediate& rel32off);
		void jmp(const Label& label);
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
		void mov(const Register& src, const Address& dst);
		void mov(const Immediate& src, const Register& dst);
		void mov(const Immediate& src, const Address& dst);
		
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
		
		void rol(const Register& reg);
		void rol(const Address& addr);
		void rol(const Register& reg, const Immediate& imm);
		void rol(const Address& addr, const Immediate& imm);
		
		void ror(const Register& reg);
		void ror(const Address& addr);
		void ror(const Register& reg, const Immediate& imm);
		void ror(const Address& addr, const Immediate& imm);
		
		void sahf() { emit(0x9e); }
		
		void shl(const Register& reg);
		void shl(const Address& addr);
		void shl(const Register& reg, const Immediate& imm);
		void shl(const Address& addr, const Immediate& imm);
		
		void shr(const Register& reg);
		void shr(const Address& addr);
		void shr(const Register& reg, const Immediate& imm);
		void shr(const Address& addr, const Immediate& imm);
		
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
};

#endif /* end of include guard: ASSEMBER_X86_64_H_OO5SJZ17 */
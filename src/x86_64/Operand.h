#ifndef OPERAND_X86_64_H_CFU26VSW
#define OPERAND_X86_64_H_CFU26VSW

#include <Operand.h>
#include <stdlib.h>

namespace x86_64 {
	class Register : public Operand {
	private:
		char m_Code;
		bool m_Extended;
	public:
		Register(char c, bool ex = false) : m_Code(c), m_Extended(ex) {}
		bool extended() const { return m_Extended; }
		char code() const { return m_Code; }
	};

	static const Register rax(0x0, false);
	static const Register rcx(0x1, false);
	static const Register rdx(0x2, false);
	static const Register rbx(0x3, false);
	static const Register rsp(0x4, false);
	static const Register rbp(0x5, false);
	static const Register rsi(0x6, false);
	static const Register rdi(0x7, false);
	static const Register r8(0x0, true);
	static const Register r9(0x1, true);
	static const Register r10(0x2, true);
	static const Register r11(0x3, true);
	static const Register r12(0x4, true);
	static const Register r13(0x5, true);
	static const Register r14(0x6, true);
	static const Register r15(0x7, true);
	
	class Immediate : public Operand {
	private:
		long long m_Data;
	public:
		Immediate(long long value) : m_Data(value) {}
		Immediate(void* ptr, size_t len);
		
		long long data() const { return m_Data; }
	};
	
	class Address : public Operand {
	private:
		const Register& m_Register;
		int32_t m_Offset;
	public:
		Address(const Register& reg, int32_t offs) : m_Register(reg), m_Offset(offs) {}
		Address(const Register& reg) : m_Register(reg), m_Offset(0) {}
		
		const Register& reg() const { return m_Register; }
		int32_t offset() const { return m_Offset; }
		
		operator const Register&() const { return reg(); }
	};
};

#endif /* end of include guard: OPERAND_X86_64_H_CFU26VSW */

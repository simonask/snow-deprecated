#ifndef OPERAND_X86_64_H_CFU26VSW
#define OPERAND_X86_64_H_CFU26VSW

#include "../Operand.h"
#include "Basic.h"
#include "lib/Value.h"
#include <stdlib.h>

namespace snow {
namespace x86_64 {
	class Register : public Operand {
	private:
		char m_Code;
		bool m_Extended;
	public:
		explicit Register(char c, bool ex = false) : m_Code(c), m_Extended(ex) {}
		bool extended() const { return m_Extended; }
		char code() const { return m_Code; }
		
		bool operator==(const Register& other) const { return m_Code == other.m_Code && m_Extended == other.m_Extended; }
		bool operator!=(const Register& other) const { return !(other == *this); }
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
		int64_t m_Data;
	public:
		template <typename T>
		Immediate(T value) : m_Data((int64_t)value) {}
		
		int64_t data() const { return m_Data; }
	};
	
	class Address : public Operand {
	private:
		const Register& m_Register;
		int32_t m_Offset;
		bool m_Wide;
	public:
		Address(const Address& other) : m_Register(other.m_Register), m_Offset(other.m_Offset), m_Wide(other.m_Wide) {}
		Address(const Register& reg, int32_t offs = 0, bool wide = true) : m_Register(reg), m_Offset(offs) {}
		
		const Register& reg() const { return m_Register; }
		int32_t offset() const { return m_Offset; }
		bool wide() const { return m_Wide; }
		
		operator const Register&() const { return reg(); }
	};
};
}

#endif /* end of include guard: OPERAND_X86_64_H_CFU26VSW */

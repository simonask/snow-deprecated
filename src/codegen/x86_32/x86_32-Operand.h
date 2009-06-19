#ifndef OPERAND_X86_32_H_CFU26VSW
#define OPERAND_X86_32_H_CFU26VSW

#include "Operand.h"
#include "base/Basic.h"
#include "runtime/Value.h"
#include <stdlib.h>

namespace snow {
namespace x86_32 {
	class Register : public Operand {
	private:
		char m_Code;
	public:
		explicit Register(char c) : m_Code(c) {}
		Register(const Register& other) : m_Code(other.m_Code) {}
		char code() const { return m_Code; }
		
		bool operator==(const Register& other) const { return m_Code == other.m_Code; }
		bool operator!=(const Register& other) const { return !(other == *this); }
	};

	static const Register eax(0x0);
	static const Register ecx(0x1);
	static const Register edx(0x2);
	static const Register ebx(0x3);
	static const Register esp(0x4);
	static const Register ebp(0x5);
	static const Register esi(0x6);
	static const Register edi(0x7);

	static const Register xmm0 = eax;
	static const Register xmm1 = ecx;
	static const Register xmm2 = edx;
	static const Register xmm3 = ebx;
	static const Register xmm4 = esp;
	static const Register xmm5 = ebp;
	static const Register xmm6 = esi;
	static const Register xmm7 = edi;
	
	class Immediate : public Operand {
	private:
		uintx m_Data;
	public:
		template <typename T>
		Immediate(T value) : m_Data((intx)value) {}
		
		uintx data() const { return m_Data; }
	};
	
	class Address : public Operand {
	private:
		const Register& m_Register;
		int32_t m_Offset;
	public:
		Address(const Address& other) : m_Register(other.m_Register), m_Offset(other.m_Offset) {}
		Address(const Register& reg, int32_t offs = 0) : m_Register(reg), m_Offset(offs) {}
		
		const Register& reg() const { return m_Register; }
		int32_t offset() const { return m_Offset; }
		
		operator const Register&() const { return reg(); }
	};
	
	class SIB : public Operand {
	public:
		enum SCALE_MODE {
			SCALE_1 = 0,
			SCALE_2 = 1,
			SCALE_4 = 2,
			SCALE_8 = 3
		};
	private:
		const Register& m_Base;
		const Register& m_Index;
		SCALE_MODE m_Scale;
		int32_t m_Offset;
	public:
		SIB(const Register& base, const Register& index, SCALE_MODE scale = SCALE_1, int32_t offset = 0) : m_Base(base), m_Index(index), m_Scale(scale), m_Offset(offset){}
		
		const Register& base() const { return m_Base; }
		const Register& index() const { return m_Index; }
		SCALE_MODE scale() const { return m_Scale; }
		int32_t offset() const { return m_Offset; }
	};
};
}

#endif /* end of include guard: OPERAND_X86_64_H_CFU26VSW */

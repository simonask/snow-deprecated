#ifndef INSTRUCTION_H_Z6LZ01ZN
#define INSTRUCTION_H_Z6LZ01ZN

#include <stdlib.h>

#include "CodeBuffer.h"
#include "Operand.h"

class ByteString;

class Instruction {
private:
	const CodeBuffer& m_Buffer;
public:
	Instruction(const CodeBuffer& buf) : m_Buffer(buf) {}
	const CodeBuffer& buffer() const { return m_Buffer; }
	size_t offset() const { return m_Buffer.offsetOf(*this); }
	virtual ByteString compile() const = 0;
	virtual size_t length() const = 0;
};

class PushInstruction : public Instruction {
private:
	const Operand& m_Operand;
public:
	PushInstruction(const CodeBuffer& buf, const Operand& o) : Instruction(buf), m_Operand(o) {}
	ByteString compile() const;
	size_t length() const;
};

class PopInstruction : public Instruction {
private:
	const Operand& m_Operand;
public:
	PopInstruction(const CodeBuffer& buf, const Operand& o) : Instruction(buf), m_Operand(o) {}
	ByteString compile() const;
	size_t length() const;
};

class RetInstruction : public Instruction {
public:
	RetInstruction(const CodeBuffer& buf) : Instruction(buf) {}
	ByteString compile() const;
	size_t length() const;
};

#endif /* end of include guard: INSTRUCTION_H_Z6LZ01ZN */

#include "CodeBuffer.h"
#include "Instruction.h"

CodeBuffer::CodeBuffer() : m_Offset(0) {
	
}

void CodeBuffer::append(const Instruction& instr) {
	m_Instructions.push_back(&instr);
	m_InstructionOffsets[&instr] = m_Offset;
	m_Offset += instr.length();
}

CodeBuffer& CodeBuffer::operator<<(const Instruction& instr) {
	append(instr);
	return *this;
}

size_t CodeBuffer::offsetOf(const Instruction& instr) const {
	return m_InstructionOffsets.find(&instr)->second;
}

ByteString CodeBuffer::compile() {
	ByteString result;
	for (std::vector<const Instruction*>::iterator iter = m_Instructions.begin(); iter != m_Instructions.end(); ++iter) {
		result.append((*iter)->compile());
	}
	return result;
}

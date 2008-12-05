#ifndef CODEBUFFER_H_ZQTXCJT2
#define CODEBUFFER_H_ZQTXCJT2

#include <vector>
#include <map>
#include "ByteString.h"

class Instruction;

class CodeBuffer {
private:
	std::vector<const Instruction*> m_Instructions;
	std::map<const Instruction*, size_t> m_InstructionOffsets;
	size_t m_Offset;
public:
	CodeBuffer();
	void append(const Instruction&);
	CodeBuffer& operator<<(const Instruction&);
	size_t offsetOf(const Instruction&) const;
	ByteString compile();
};

#endif /* end of include guard: CODEBUFFER_H_ZQTXCJT2 */

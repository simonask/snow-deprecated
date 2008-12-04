#include "Operand-x86_64.h"

#include <string.h>

namespace x86_64 {
	Immediate::Immediate(long long value) : m_Long(true) {
		memcpy(m_Data, &value, 8);
	}
	
	Immediate::Immediate(int value) : m_Long(false) {
		memcpy(m_Data, &value, 4);
	}
	
	Immediate::Immediate(void* ptr, bool is_long) : m_Long(is_long) {
		memcpy(m_Data, ptr, size());
	}
}

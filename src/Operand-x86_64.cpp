#include "Operand-x86_64.h"

#include <string.h>

#include <math.h>

namespace x86_64 {
	Immediate::Immediate(void* ptr, size_t len) : m_Data(0) {
		memcpy(&m_Data, ptr, len < 8 ? len : 8);
	}
}

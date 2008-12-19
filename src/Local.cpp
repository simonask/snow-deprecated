#include "Local.h"
#include <sstream>

namespace snot {
	Local::Local(int index, const char* name) : m_StackIndex(index) {
		if (name)
			m_Name = name;
		else {
			std::stringstream ss;
			ss << "unnamed";
			ss << ms_UnnamedCounter++;
			m_Name = ss.str();
		}
	}
	
	int Local::ms_UnnamedCounter = 0;
}
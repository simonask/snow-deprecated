#ifndef STRING_H_HLAIM4GC
#define STRING_H_HLAIM4GC

#include "Object.h"
#include <string>

namespace snot {
	Object* string_prototype();
	
	class String : public Object {
	private:
		std::string m_String;
	public:
		String(const std::string& str) : m_String(str) {}
		String() : Object(string_prototype()) {}
		VALUE copy() const { return value(new String(*this)); }
		
		operator const std::string&() const { return m_String; }
		operator const char*() const { return m_String.c_str(); }
	};
}

#endif /* end of include guard: STRING_H_HLAIM4GC */

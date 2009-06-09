#ifndef STRING_H_HLAIM4GC
#define STRING_H_HLAIM4GC

#include "Object.h"
#include "Runtime.h"
#include <string>
#include <sstream>

#include "3rdparty/oniguruma.h"

namespace snow {
	Object* string_prototype();
	void string_prototype_init();
	
	class String : public ThinObject {
	private:
		std::string m_String;
	public:
		NO_INIT;

		String() : ThinObject(string_prototype()), m_String("") {}
		String(const std::string& str) : ThinObject(string_prototype()), m_String(str) {}
		template <typename... A>
		String(const char* format, A... args) : ThinObject(string_prototype()) {
			std::stringstream ss;
			build_string(ss, format, args...);
			m_String = ss.str();
		}
		IObject* copy() const { return new String(*this); }
		
		const std::string& str() const { return m_String; }
		operator const std::string&() const { return str(); }
		operator const char*() const { return m_String.c_str(); }
		operator const OnigUChar*() const { return (const OnigUChar*)m_String.c_str(); }

		bool operator==(const String& other) const { return m_String == other.m_String; }

		int length() const { return m_String.size(); }
		
	private:
		void build_string(std::stringstream& ss, const char* format) {
			ss << format;
		}
		
		template <typename... A>
		void build_string(std::stringstream& ss, const char* format, VALUE val, A... args) {
			while (*format) {
				if (*format == '%' && *++format != '%') {
					ss << value_to_string(val);
					return build_string(ss, format, args...);
				}
				ss << *format++;
			}
		}
	};
}

#endif /* end of include guard: STRING_H_HLAIM4GC */

#ifndef STRING_H_HLAIM4GC
#define STRING_H_HLAIM4GC

#include "Object.h"
#include "Runtime.h"
#include "Array.h"
#include <string>
#include <sstream>

namespace snow {
	Ptr<Object> string_prototype();
	void string_prototype_init();
	
	class String : public ThinObject {
	public:
		static const char* EMPTY;
	private:
		DataPtr<char> m_Data;
		GC_ROOTS { GC_SUPER(ThinObject); GC_ROOT(m_Data); }
	public:
		template <typename... A>
		String(A... args) : ThinObject(string_prototype()), m_Data(const_cast<char*>(EMPTY)) {}

		void initialize() {}
		void initialize(const String& other);
		void initialize(const std::string& str);
		void initialize(const char* str);
		template <typename... A>
		void initialize(const char* fmt, const A&... args);

		Ptr<IObject> copy() const { return gc_new<String>(*this); }
		const char* c_str() const { return m_Data; }

		bool operator==(const String& other) const { return !strcmp(m_Data, other.m_Data); }
		bool operator!=(const String& other) const { return !(*this == other); }

		int length() const { return strlen(m_Data); }

		Ptr<String> add_p(const Ptr<String>& other);
		Ptr<String> reverse_p();
		Ptr<String> substring_p(uintx index, uintx length);

	private:
		template <typename... Args>
		void convert_args_to_strings(Ptr<Array>& array, const Value& val, const Args&...);
		
		void convert_args_to_strings(Ptr<Array>& array) {}
		void interpolate_internal(Handle<String>& format, Handle<Array>&); 
	};

	inline void String::initialize(const std::string& str) {
		initialize(str.c_str());
	}

	inline void String::initialize(const String& other) {
		initialize((const char*)other.m_Data);
	}

	inline void String::initialize(const char* str) {
		uintx len = strlen(str);
		m_Data = gc_new_array<char>(len + 1);
		memcpy(m_Data, str, len);
		m_Data[len] = '\0';
	}

	template <typename... A>
	void String::initialize(const char* format, const A&... args) {
		HandleScope _;
		Handle<char> fmt = const_cast<char*>(format); // hope that format isn't a substring of another gc'd String...
		Handle<String> format_str = gc_new<String>((const char*)fmt);
		Handle<Array> strings = gc_new<Array>();
		convert_args_to_strings(strings, args...);
		interpolate_internal(format_str, strings);
	}

	template <typename... A>
	void String::convert_args_to_strings(Ptr<Array>& array, const Value& val, const A&... args) {
		array->push(snow::call_method(val, "to_string"));
		convert_args_to_strings(array, args...);
	}

	inline std::ostream& operator<<(std::ostream& os, const String& str) {
		os << str.c_str();
		return os;
	}
}

#endif /* end of include guard: STRING_H_HLAIM4GC */

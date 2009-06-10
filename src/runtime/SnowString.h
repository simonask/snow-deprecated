#ifndef STRING_H_HLAIM4GC
#define STRING_H_HLAIM4GC

#include "Object.h"
#include "Runtime.h"
#include "Array.h"
#include <string>
#include <sstream>

namespace snow {
	Object* string_prototype();
	void string_prototype_init();
	
	class String : public ThinObject {
	public:
		static const char* EMPTY;
	private:
		char* m_Data;
		GC_ROOTS { GC_SUPER(ThinObject); GC_ROOT(m_Data); }
	public:
		template <typename... A>
		String(A... args) : ThinObject(string_prototype()), m_Data(const_cast<char*>(EMPTY)) {}

		void initialize() {}
		void initialize(const String& other);
		void initialize(const std::string& str);
		void initialize(const char* str);
		template <typename... A>
		void initialize(const char* fmt, A... args);

		IObject* copy() const { return gc_new<String>(*this); }
		const char* c_str() const { return m_Data; }

		bool operator==(const String& other) const { return !strcmp(m_Data, other.m_Data); }
		bool operator!=(const String& other) const { return !(*this == other); }

		int length() const { return strlen(m_Data); }

		String* add_p(String* other);
		String* reverse_p();
		String* substring_p(size_t index, size_t length);

	private:
		void interpolate_internal(Handle<String>& format, Handle<Array>&); 

		template <typename T, typename... A>
		static void convert_args_to_strings(Handle<Array>& array, T val, A... args);
		template <typename T>
		static void convert_args_to_strings(Handle<Array>& array, T val);
		template <typename T>
		static void convert_args_to_strings(Handle<Array>& array, const Handle<T>& val);
	};

	inline void String::initialize(const std::string& str) {
		initialize(str.c_str());
	}

	inline void String::initialize(const String& other) {
		initialize(other.m_Data);
	}

	inline void String::initialize(const char* str) {
		size_t len = strlen(str);
		m_Data = gc_new_array<char>(len + 1);
		memcpy(m_Data, str, len);
		m_Data[len] = '\0';
	}

	template <typename... A>
	void String::initialize(const char* format, A... args) {
		HandleScope _;
		Handle<char> fmt = const_cast<char*>(format); // hope that format isn't a substring of another gc'd String...
		Handle<String> format_str = gc_new<String>((const char*)fmt);
		Handle<Array> strings = gc_new<Array>();
		convert_args_to_strings(strings, args...);
		interpolate_internal(format_str, strings);

}

	template <typename T, typename... A>
	void String::convert_args_to_strings(Handle<Array>& array, T val, A... args) {
		convert_args_to_strings(array, val);
		convert_args_to_strings(array, args...);
	}

	template <typename T>
	void String::convert_args_to_strings(Handle<Array>& array, T val) {
		std::stringstream ss;
		ss << val;
		array->push(gc_new<String>(ss.str()));
	}

	template <>
	inline void String::convert_args_to_strings(Handle<Array>& array, VALUE val) {
		array->push(snow::call_method(val, "to_string", 0));
	}

	template <typename T>
	void String::convert_args_to_strings(Handle<Array>& array, const Handle<T>& val) {
		convert_args_to_strings(array, val.value());
	}

	template <>
	inline void String::convert_args_to_strings(Handle<Array>& array, const ValueHandle& val) {
		convert_args_to_strings(array, val.value());
	}


	inline std::ostream& operator<<(std::ostream& os, const String& str) {
		os << str.c_str();
		return os;
	}
}

#endif /* end of include guard: STRING_H_HLAIM4GC */

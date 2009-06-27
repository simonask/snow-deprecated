#include <sstream>
#include "SnowString.h"
#include "Runtime.h"
#include "Function.h"
#include "Exception.h"

namespace snow {
	const char* String::EMPTY = "";

	void String::interpolate_internal(Handle<String>& format, Handle<Array>& strings)
	{
		HandleScope();

		uintx result_length = format->length();
		for (uintx i = 0; i < strings->length(); ++i) {
			Handle<String> current = object_cast<String>((*strings)[i]);
			ASSERT(current && "Some object's to_string returning a non-string.");
			result_length += current->length() - 1; // subtract 1 because of the % being replaced.
		}
		m_Data = gc_new_array<char>(result_length + 1);

		uintx i = 0;
		const char* c = format->c_str();
		uintx next_string_index = 0;
		while (*c) {
			ASSERT(i <= result_length && "Calculated result length wrong.");
			if (c[0] == '%' && c[1] != '%' && next_string_index < strings->length()) {
				Handle<String> current = object_cast<String>((*strings)[next_string_index]);
				uintx len = current->length();
				memcpy(&m_Data[i], current->m_Data, len);
				i += len;
				++next_string_index;
			}
			else
			{
				m_Data[i] = *c;
				++i;
			}
			++c;
		}
		m_Data[result_length] = '\0'; 
	}

	Ptr<String> String::add_p(const Ptr<String>& other) {
		HandleScope _;
		Handle<String> THIS = this;
		Handle<String> OTHER = other;
		uintx first_len = length();
		uintx second_len = other->length();
		uintx combined_len = first_len + second_len;
		Handle<char> data = gc_new_array<char>(combined_len+1);
		memcpy(data.value(), THIS->m_Data.value(), first_len);
		memcpy(&data[first_len], OTHER->m_Data.value(), second_len);
		data[combined_len] = '\0';
		Handle<String> result = gc_new<String>();
		result->m_Data = data;
		return result;
	}

	Ptr<String> String::reverse_p() {
		HandleScope _;
		uintx len = length();
		Handle<String> THIS = this;
		Handle<char> data = gc_new_array<char>(len + 1);
		Handle<char> old_data = THIS->m_Data;

		// TODO: Unicode
		for (uintx i = 0; i < len; ++i) {
			data[len-i-1] = old_data[i];
		}
		data[len] = '\0';

		Handle<String> result = gc_new<String>();
		result->m_Data = data;
		return result;
	}

	Ptr<String> String::substring_p(uintx offset, uintx len) {
		HandleScope();
		Handle<String> THIS = this;
		Handle<String> result = gc_new<String>();

		uintx own_length = THIS->length();
		if (offset >= own_length)
			return result;
		if (offset + len >= own_length)
		{
			len = own_length - offset;
		}
		result->m_Data = gc_new_array<char>(len + 1);
		memcpy(result->m_Data, &THIS->m_Data[offset], len);
		result->m_Data[len] = '\0';
		return result;
	}

	static Value string_to_string(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, String);
		return self;
	}

	static Value string_inspect(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		Handle<String> str = object_cast<String>(self);
		ASSERT(str);
		// TODO: Escape properly
		return gc_new<String>("\"%\"", str);
	}
	
	static Value string_equals(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, String);
		ASSERT_ARGS(args.size >= 1);
		Handle<String> str = object_cast<String>(self);
		if (str.value() == args.data[0])
			return value(true);
		else {
			Handle<String> other = object_cast<String>(args.data[0]);
			return value(other && *str == *other);
		}
	}
	
	static Value string_plus(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, String);
		ASSERT_ARGS(args.size == 1);
		
		Handle<String> str = object_cast<String>(self);
		Handle<String> other = object_cast<String>(snow::call_method(args.data[0], "to_string"));
		if (!other)
		{
			throw_exception(gc_new<String>("to_string returned non-string."));
		}
		return str->add_p(other);
	}
	
	static Value string_reverse(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		Handle<String> SELF = object_cast<String>(self);
		ASSERT(SELF);
		ASSERT_ARGS(args.size == 0);

		return SELF->reverse_p();
	}
	
	Ptr<Object> string_prototype() {
		static Ptr<Object> proto;
		if (proto) return proto;
	 	proto = malloc_new<Object>();
		return proto;
	}

	void string_prototype_init() {
		Ptr<Object> proto = string_prototype();
		proto->set_raw("name", gc_new<String>("String"));
		proto->set_raw("to_string", gc_new<Function>(string_to_string));
		proto->set_raw("inspect", gc_new<Function>(string_inspect));
		proto->set_raw("=", gc_new<Function>(string_equals));
		proto->set_raw("+", gc_new<Function>(string_plus));
		proto->set_raw("<<", gc_new<Function>(string_plus));
		proto->set_raw("reverse", gc_new<Function>(string_reverse));
	}
}

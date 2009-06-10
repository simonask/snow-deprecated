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

		size_t result_length = format->length();
		for (size_t i = 0; i < strings->length(); ++i) {
			Handle<String> current = object_cast<String>((*strings)[i]);
			ASSERT(current && "Some object's to_string returning a non-string.");
			result_length += current->length() - 1; // subtract 1 because of the % being replaced.
		}
		m_Data = gc_new_array<char>(result_length + 1);

		size_t i = 0;
		const char* c = format->c_str();
		size_t next_string_index = 0;
		while (*c) {
			ASSERT(i <= result_length && "Calculated result length wrong.");
			if (c[0] == '%' && c[1] != '%' && next_string_index < strings->length()) {
				Handle<String> current = object_cast<String>((*strings)[next_string_index]);
				size_t len = current->length();
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

	String* String::add_p(String* other) {
		HandleScope _;
		Handle<String> THIS = this;
		Handle<String> OTHER = other;
		size_t first_len = length();
		size_t second_len = other->length();
		size_t combined_len = first_len + second_len;
		Handle<char> data = gc_new_array<char>(combined_len+1);
		memcpy(data, THIS->m_Data, first_len);
		memcpy(&data[first_len], OTHER->m_Data, second_len);
		data[combined_len] = '\0';
		Handle<String> result = gc_new<String>();
		result->m_Data = data;
		return result;
	}

	String* String::reverse_p() {
		HandleScope _;
		size_t len = length();
		Handle<String> THIS = this;
		Handle<char> data = gc_new_array<char>(len + 1);
		Handle<char> old_data = THIS->m_Data;

		// TODO: Unicode
		for (size_t i = 0; i < len; ++i) {
			data[len-i-1] = old_data[i];
		}
		data[len] = '\0';

		Handle<String> result = gc_new<String>();
		result->m_Data = data;
		return result;
	}

	String* String::substring_p(size_t offset, size_t len) {
		HandleScope();
		Handle<String> THIS = this;
		Handle<String> result = gc_new<String>();

		size_t own_length = THIS->length();
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

	static VALUE string_to_string(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, String);
		return self;
	}

	static VALUE string_inspect(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		Handle<String> str = object_cast<String>(self);
		ASSERT(str);
		// TODO: Escape properly
		return gc_new<String>("\"%\"", str);
	}
	
	static VALUE string_equals(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, String);
		ASSERT_ARGS(num_args >= 1);
		auto str = object_cast<String>(self);
		if (args[0] == str)
			return value(true);
		else {
			Handle<String> other = object_cast<String>(args[0]);
			return value(other && *str == *other);
		}
	}
	
	static VALUE string_plus(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, String);
		ASSERT_ARGS(num_args == 1);
		
		Handle<String> str = object_cast<String>(self);
		Handle<String> other = object_cast<String>(snow::call_method(args[0], "to_string", 0));
		if (!other)
		{
			throw_exception(gc_new<String>("to_string returned non-string."));
		}
		return str->add_p(other);
	}
	
	static VALUE string_reverse(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		Handle<String> SELF = object_cast<String>(self);
		ASSERT(SELF);
		ASSERT_ARGS(num_args == 0);

		return SELF->reverse_p();
	}
	
	Object* string_prototype() {
		static Object* proto = NULL;
		if (proto) return proto;
	 	proto = malloc_new<Object>();
		return proto;
	}

	void string_prototype_init() {
		Object* proto = string_prototype();
		proto->set_raw_s("name", gc_new<String>("String"));
		proto->set_raw_s("to_string", gc_new<Function>(string_to_string));
		proto->set_raw_s("inspect", gc_new<Function>(string_inspect));
		proto->set_raw_s("=", gc_new<Function>(string_equals));
		proto->set_raw_s("+", gc_new<Function>(string_plus));
		proto->set_raw_s("<<", gc_new<Function>(string_plus));
		proto->set_raw_s("reverse", gc_new<Function>(string_reverse));
	}
}

#include "Array.h"
#include "Runtime.h"
#include "SnowString.h"
#include "Function.h"
#include <sstream>

#define DEFAULT_ARRAY_LENGTH 8

namespace snow {
	GC_ROOTS_IMPL(Array) {
		GC_SUPER(Object);
		
		// If m_AllocatedSize == 0, this is a wrapper around an external array.
		if (m_AllocatedSize)
			GC_ROOT(m_Data);

		for (size_t i = 0; i < m_Length; ++i) {
			GC_ROOT(m_Data[i]);
		}
	}

	void Array::resize(size_t new_size) {
		// TODO: Grow more than strictly necessary to minimize allocations

		// This function contains an allocation, which might trigger a garbage collection,
		// so we need to guard the "this" pointer against this. This would be nice to improve.

		HandleScope _;
		Handle<Array> self = this;

		VALUE* old_pointer = self->m_Data;
		self->m_Data = new(kGarbage, kBlob) VALUE[new_size];
		self->m_AllocatedSize = new_size;
		if (self->m_Length != 0 && old_pointer) {
			memcpy(self->m_Data, old_pointer, self->m_Length*sizeof(VALUE));
		}
	}
	
	Array::Array(size_t preallocate_length) : Object(array_prototype()), m_Data(NULL), m_Length(0), m_AllocatedSize(0) {
		if (preallocate_length == 0)
			preallocate_length = DEFAULT_ARRAY_LENGTH;
		resize(preallocate_length * sizeof(VALUE));
	}
	
	Array::Array(const Array& other) : Object(array_prototype()), m_Data(other.m_Data), m_Length(other.m_Length), m_AllocatedSize(0) {
		resize(m_Length * sizeof(VALUE));
	}
	
	Array::Array(VALUE* existing_array, size_t len, bool copy) : Object(array_prototype()), m_Data(existing_array), m_Length(len), m_AllocatedSize(0) {
		if (copy)
			resize(m_Length * sizeof(VALUE));
	}
	
	void Array::ensure_length(size_t len) {
		ASSERT(!is_frozen());
		if (len * sizeof(VALUE) <= m_AllocatedSize)
			return;
		resize(len * sizeof(VALUE));
	}
	
	VALUE& Array::operator[](int64_t idx) {
		if (idx < 0)
			idx %= m_Length;
		if (idx >= (int64_t)m_Length)
		{
			ensure_length(idx+1);
			auto old_length = m_Length;
			m_Length = idx+1;
			for (size_t i = old_length; i < m_Length; ++i) {
				m_Data[i] = nil();
			}
		}
		return m_Data[idx];
	}
	
	VALUE Array::operator[](int64_t idx) const {
		if (idx < 0)
			idx = m_Length + idx;
		if (idx < 0 || idx >= (int64_t)m_Length)
			return nil();
		return m_Data[idx];
	}
	
	VALUE Array::push(VALUE val) {
		ensure_length(m_Length + 1);
		m_Data[m_Length] = val;
		++m_Length;
		return val;
	}
	
	VALUE Array::pop() {
		ASSERT(!is_frozen());
		if (m_Length == 0)
			return nil();
		VALUE val = m_Data[m_Length-1];
		--m_Length;
		return val;
	}
	
	VALUE Array::unshift(VALUE val) {
		ensure_length(m_Length+1);
		// XXX: For some reason, memmove doesn't work here. Investigate!
		// Also, this might be acceptable, since it's faster than memmove.
		for (size_t i = 0; i < m_Length; ++i) {
			m_Data[m_Length-i] = m_Data[m_Length-1-i];
		}
		++m_Length;
		m_Data[0] = val;
		return val;
	}
	
	VALUE Array::shift() {
		ASSERT(!is_frozen());
		if (m_Length == 0)
			return nil();
		VALUE val = m_Data[0];
		--m_Length;
		for (size_t i = 0; i < m_Length; ++i) {
			m_Data[i] = m_Data[i+1];
		}
		return val;
	}
	
	VALUE Array::va_call(VALUE self, uint64_t num_args, va_list& args) {
		if (num_args == 0)
			return this;
		if (num_args == 1) {
			int64_t index = integer(va_arg(args, VALUE));
			return (*this)[index];
		}
		// TODO: ranges and such.
		TRAP();
		return this;
	}
	
	static VALUE array_new(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		return new Array(args, num_args);
	}
	
	static VALUE array_get(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(num_args == 1);
		auto array = object_cast<Array>(self);
		int64_t idx = integer(args[0]);
		return (*array)[idx];
	}
	
	static VALUE array_set(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(num_args == 2);
		auto array = object_cast<Array>(self);
		int idx = integer(args[0]);
		VALUE new_value = args[1];
		return array->set_by_index(idx, new_value);
	}
	
	static VALUE array_each(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(num_args >= 1);
		Handle<Array> array = object_cast<Array>(self);
		
		ValueHandle closure = args[0];
		for (size_t i = 0; i < array->length(); ++i) {
			snow::call(NULL, closure, 2, (*array)[i], value((int64_t)i));
		}
		return self;
	}

	static VALUE array_each_parallel(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(num_args >= 1);
		Handle<Array> array = object_cast<Array>(self);

		ValueHandle closure = args[0];
		#pragma omp parallel for
		for (size_t i = 0; i < array->length(); ++i) {
			snow::call(NULL, closure, 2, (*array)[i], value((int64_t)i));
		}
		return self;
	}
	
	static VALUE array_length(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		auto array = object_cast<Array>(self);
		return value((int64_t)array->length());
	}
	
	static VALUE array_inspect(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		auto array = object_cast<Array>(self);
		std::stringstream ss;
		
		ss << "@(";
		for (size_t i = 0; i < array->length(); ++i) {
			ss << value_to_string(snow::call_method((*array)[i], "inspect", 0));
			if (i != array->length() - 1)
				ss << ", ";
		}
		ss << ")";
		
		return new String(ss.str());
	}
	
	static VALUE array_push(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(num_args == 1);
		auto array = object_cast<Array>(self);
		VALUE val = args[0];
		array->push(val);
		return self;
	}
	
	static VALUE array_pop(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(num_args == 0);
		auto array = object_cast<Array>(self);
		return array->pop();
	}
	
	static VALUE array_unshift(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(num_args == 1);
		auto array = object_cast<Array>(self);
		VALUE val = args[0];
		return array->unshift(val);
	}
	
	static VALUE array_shift(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(num_args == 0);
		auto array = object_cast<Array>(self);
		return array->shift();
	}
	
	Object* array_prototype() {
		static Object* proto = NULL;
		if (proto) return proto;
		proto = new(kMalloc) Object;
		proto->set_raw_s("new", new Function(array_new));
		proto->set_raw_s("get", new Function(array_get));
		proto->set_raw_s("set", new Function(array_set));
		proto->set_raw_s("each", new Function(array_each));
		proto->set_raw_s("each_parallel", new Function(array_each_parallel));
		proto->set_raw_s("push", new Function(array_push));		
		proto->set_raw_s("pop", new Function(array_pop));
		proto->set_raw_s("unshift", new Function(array_unshift));
		proto->set_raw_s("shift", new Function(array_shift));
		proto->set_property_getter(symbol("length"), new Function(array_length));
		proto->set_raw_s("inspect", new Function(array_inspect));
		proto->set_raw_s("<<", new Function(array_push));
		return proto;
	}
}

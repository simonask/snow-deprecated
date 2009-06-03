#include "Array.h"
#include "Runtime.h"
#include "SnowString.h"
#include "Function.h"
#include <sstream>

#define DEFAULT_ARRAY_LENGTH 8

namespace snow {
	GC_ROOTS_IMPL(Array) {
		GC_SUPER(Object);
		
		// If m_AllocatedLength == 0, this is a wrapper around an external array.
		if (m_AllocatedLength)
			GC_ROOT(m_Data);

		for (size_t i = 0; i < m_Length; ++i) {
			GC_ROOT(m_Data[i]);
		}
	}

	void Array::resize(size_t new_size) {
		if (m_AllocatedLength >= new_size)
			return;

		// TODO: Grow more than strictly necessary to minimize allocations

		// This function contains an allocation, which might trigger a garbage collection,
		// so we need to guard the "this" pointer against this. This would be nice to improve.

		HandleScope _;
		Handle<Array> self = this;

		VALUE* old_pointer = self->m_Data;
		self->m_Data = new(kGarbage, kBlob) VALUE[new_size];
		self->m_AllocatedLength = new_size;
		if (self->m_Length != 0 && old_pointer) {
			memcpy(self->m_Data, old_pointer, self->m_Length*sizeof(VALUE));
		}
	}

	void Array::preallocate(size_t len) {
		if (m_Length < len) {
			resize(len);
		}
	}
	
	Array::Array() : Object(array_prototype()), m_Data(NULL), m_Length(0), m_AllocatedLength(0) {}
	
	VALUE& Array::operator[](int64_t idx) {
		HandleScope _;
		Handle<Array> self = this;

		if (idx < 0)
			idx %= self->m_Length;
		if (idx >= (int64_t)self->m_Length)
		{
			self->preallocate(idx+1);
			auto old_length = self->m_Length;
			self->m_Length = idx+1;
			for (size_t i = old_length; i < self->m_Length; ++i) {
				self->m_Data[i] = nil();
			}
		}
		return self->m_Data[idx];
	}
	
	VALUE Array::operator[](int64_t idx) const {
		if (idx < 0)
			idx = m_Length + idx;
		if (idx < 0 || idx >= (int64_t)m_Length)
			return nil();
		return m_Data[idx];
	}
	
	VALUE Array::push(VALUE val) {
		HandleScope _;
		Handle<Array> self = this;

		self->preallocate(m_Length + 1);
		self->m_Data[self->m_Length] = val;
		++self->m_Length;
		return self;
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
		HandleScope _;
		Handle<Array> self = this;
		
		self->preallocate(m_Length+1);
		// XXX: For some reason, memmove doesn't work here. Investigate!
		// Also, this might be acceptable, since it's faster than memmove.
		for (size_t i = 0; i < self->m_Length; ++i) {
			self->m_Data[self->m_Length-i] = self->m_Data[self->m_Length-1-i];
		}
		++self->m_Length;
		self->m_Data[0] = val;
		return self;
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

	void Array::set_data(VALUE* data, size_t len) {
		preallocate(len);
		memcpy(m_Data, data, len*sizeof(VALUE));
		m_Length = len;
	}

	void Array::set_reference(VALUE* data, size_t len) {
		m_AllocatedLength = 0;
		m_Data = len ? data : NULL;
		m_Length = len;
	}

	Array* Array::copy(VALUE* array, size_t len) {
		HandleScope _;
		Handle<Array> self = new Array;
		self->set_data(array, len);
		return self;
	}

	Array* Array::copy(const Array& array) {
		return copy(array.m_Data, array.m_Length);
	}

	Array* Array::reference(VALUE* array, size_t len) {
		HandleScope _;
		Handle<Array> self = new Array;
		self->set_reference(array, len);
		return self;
	}

	Array* Array::reference(const Array& array) {
		return reference(array.m_Data, array.m_Length);
	}
	
	static VALUE array_new(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		if (num_args)
			return Array::copy(args, num_args);
		return new Array();
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

	static VALUE array_map(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(num_args >= 1);
		Handle<Array> array = object_cast<Array>(self);

		ValueHandle closure = args[0];
		Handle<Array> result = new Array;
		result->preallocate(result->length());
		for (size_t i = 0; i < array->length(); ++i) {
			(*result)[i] = snow::call(NULL, closure, 2, (*array)[i], value((int64_t)i));
		}
		return result;
	}

	static VALUE array_map_parallel(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(num_args >= 1);
		Handle<Array> array = object_cast<Array>(self);

		ValueHandle closure = args[0];
		Handle<Array> result = new Array;
		result->preallocate(result->length());
		#pragma omp parallel for shared(result)
		for (size_t i = 0; i < array->length(); ++i) {
			(*result)[i] = snow::call(NULL, closure, 2, (*array)[i], value((int64_t)i));
		}
		return result;
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
		proto->set_raw_s("map", new Function(array_map));
		proto->set_raw_s("map_parallel", new Function(array_map_parallel));
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

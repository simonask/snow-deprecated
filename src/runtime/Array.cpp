#include "Array.h"
#include "Runtime.h"
#include "SnowString.h"
#include "Function.h"
#include "Arguments.h"
#include "Exception.h"
#include <sstream>

#define DEFAULT_ARRAY_LENGTH 8

namespace snow {
	GC_ROOTS_IMPL(Array) {
		GC_SUPER(Object);
		
		// If m_AllocatedLength == 0, this is a wrapper around an external array.
		if (m_AllocatedLength)
			GC_ROOT(m_Data);

		for (uintx i = 0; i < m_Length; ++i) {
			GC_ROOT(m_Data[i]);
		}
	}

	void Array::resize(uintx new_size) {
		if (m_AllocatedLength >= new_size)
			return;

		// TODO: Grow more than strictly necessary to minimize allocations

		// This function contains an allocation, which might trigger a garbage collection,
		// so we need to guard the "this" pointer against this. This would be nice to improve.

		HandleScope _;
		Handle<Array> self = this;

		Handle<VALUE> old_pointer = self->m_Data;
		self->m_Data = gc_new_array<VALUE>(new_size);
		self->m_AllocatedLength = new_size;
		if (self->m_Length != 0 && old_pointer) {
			memcpy(self->m_Data.value(), old_pointer.value(), self->m_Length*sizeof(VALUE));
		}
	}

	void Array::preallocate(uintx len) {
		if (m_Length < len) {
			resize(len);
		}
	}
	
	Array::Array() : Object(array_prototype()), m_Length(0), m_AllocatedLength(0) {}
	
	VALUE& Array::operator[](intx idx) {
		HandleScope _;
		Handle<Array> self = this;
		
		if (idx < 0) {
			if ((size_t)abs(idx) <= self->m_Length)
				idx = self->m_Length + idx;
			else
				throw_exception(gc_new<String>("Out of bounds %", value(idx)));
		}
		if (idx >= (intx)self->m_Length) {
			self->preallocate(idx+1);
			auto old_length = self->m_Length;
			self->m_Length = idx+1;
			for (uintx i = old_length; i < self->m_Length; ++i) {
				self->m_Data[i] = nil();
			}
		}
		
		return self->m_Data[idx];
	}
	
	Value Array::operator[](intx idx) const {
		if (idx < 0)
			idx = m_Length + idx;
		if (idx < 0 || idx >= (intx)m_Length)
			return nil();
		return m_Data[idx];
	}
	
	Value Array::push(const Value& val) {
		HandleScope _;
		Handle<Array> self = this;

		self->preallocate(m_Length + 1);
		self->m_Data[self->m_Length] = val.value();
		++self->m_Length;
		return self;
	}
	
	Value Array::pop() {
		ASSERT(!is_frozen());
		if (m_Length == 0)
			return nil();
		VALUE val = m_Data[m_Length-1];
		--m_Length;
		return val;
	}
	
	Value Array::unshift(const Value& val) {
		HandleScope _;
		Handle<Array> self = this;
		
		self->preallocate(m_Length+1);
		// XXX: For some reason, memmove doesn't work here. Investigate!
		// Also, this might be acceptable, since it's faster than memmove.
		for (uintx i = 0; i < self->m_Length; ++i) {
			self->m_Data[self->m_Length-i] = self->m_Data[self->m_Length-1-i];
		}
		++self->m_Length;
		(self->m_Data)[0] = val.value();
		return self;
	}
	
	Value Array::shift() {
		ASSERT(!is_frozen());
		if (m_Length == 0)
			return nil();
		Value val = m_Data[0];
		--m_Length;
		for (uintx i = 0; i < m_Length; ++i) {
			m_Data[i] = m_Data[i+1];
		}
		return val;
	}
	
	Value Array::call(const Value& self, const Arguments& args) {
		if (args.size == 0)
			return this;
		if (args.size == 1) {
			intx index = integer(args.data[0]);
			return (*this)[index];
		}
		// TODO: ranges and such.
		TRAP();
		return this;
	}

	void Array::set_data(const DataPtr<VALUE>& data, uintx len) {
		HandleScope _;
		Handle<Array> THIS = this;
		THIS->preallocate(len);
		memcpy(THIS->m_Data.value(), data.value(), len*sizeof(VALUE));
		THIS->m_Length = len;
	}

	void Array::set_reference(const DataPtr<VALUE>& data, uintx len) {
		m_AllocatedLength = 0;
		m_Data = len ? data.value() : NULL;
		m_Length = len;
	}

	Ptr<Array> Array::copy(const DataPtr<VALUE>& array, uintx len) {
		HandleScope _;
		Handle<Array> self = gc_new<Array>();
		self->set_data(array, len);
		return self;
	}

	Ptr<Array> Array::copy(const Array& array) {
		return copy(array.m_Data, array.m_Length);
	}

	Ptr<Array> Array::reference(const DataPtr<VALUE>& array, uintx len) {
		HandleScope _;
		Handle<Array> self = gc_new<Array>();
		self->set_reference(array, len);
		return self;
	}

	Ptr<Array> Array::reference(const Array& array) {
		return reference(array.m_Data, array.m_Length);
	}
	
	static Value array_new(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		if (args.size)
			return Array::copy(args.data, args.size);
		return gc_new<Array>();
	}
	
	static Value array_get(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(args.size == 1);
		auto array = object_cast<Array>(self);
		intx idx = integer(args.data[0]);
		return (*array)[idx];
	}
	
	static Value array_set(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(args.size == 2);
		auto array = object_cast<Array>(self);
		int idx = integer(args.data[0]);
		VALUE new_value = args.data[1];
		return array->set_by_index(idx, new_value);
	}
	
	static Value array_each(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(args.size >= 1);
		Handle<Array> array = object_cast<Array>(self);
		
		ValueHandle closure = args.data[0];
		for (uintx i = 0; i < array->length(); ++i) {
			snow::call(NULL, closure, (*array)[i], value((intx)i));
		}
		return self;
	}

	static Value array_each_parallel(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(args.size >= 1);
		Handle<Array> array = object_cast<Array>(self);

		ValueHandle closure = args.data[0];
		#pragma omp parallel for
		for (uintx i = 0; i < array->length(); ++i) {
			snow::call(NULL, closure, (*array)[i], value((intx)i));
		}
		return self;
	}

	static Value array_map(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(args.size >= 1);
		Handle<Array> array = object_cast<Array>(self);

		ValueHandle closure = args.data[0];
		Handle<Array> result = gc_new<Array>();
		result->preallocate(result->length());
		for (uintx i = 0; i < array->length(); ++i) {
			(*result)[i] = snow::call(NULL, closure, (*array)[i], value((intx)i)).value();
		}
		return result;
	}

	static Value array_map_parallel(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(args.size >= 1);
		Handle<Array> array = object_cast<Array>(self);

		ValueHandle closure = args.data[0];
		Handle<Array> result = gc_new<Array>();
		result->preallocate(result->length());
		uintx len = array->length();
		#pragma omp parallel for
		for (uintx i = 0; i < len; ++i) {
			Value transformed = snow::call(NULL, closure, (*array)[i], value((intx)i));
			(*result)[i] = transformed.value();
		}
		return result;
	}
	
	static Value array_length(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		auto array = object_cast<Array>(self);
		return value((intx)array->length());
	}
	
	static Value array_inspect(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		auto array = object_cast<Array>(self);
		std::stringstream ss;
		
		ss << "@(";
		for (uintx i = 0; i < array->length(); ++i) {
			ss << value_to_string(snow::call_method((*array)[i], "inspect"));
			if (i != array->length() - 1)
				ss << ", ";
		}
		ss << ")";
		
		return gc_new<String>(ss.str());
	}
	
	static Value array_push(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(args.size == 1);
		auto array = object_cast<Array>(self);
		VALUE val = args.data[0];
		array->push(val);
		return self;
	}
	
	static Value array_pop(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(args.size == 0);
		auto array = object_cast<Array>(self);
		return array->pop();
	}
	
	static Value array_unshift(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(args.size == 1);
		auto array = object_cast<Array>(self);
		VALUE val = args.data[0];
		return array->unshift(val);
	}
	
	static Value array_shift(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(args.size == 0);
		auto array = object_cast<Array>(self);
		return array->shift();
	}
	
	Ptr<Object> array_prototype() {
		static Ptr<Object> proto;
		if (proto) return proto;
		proto = malloc_new<Object>();
		return proto;
	}

	void array_prototype_init() {
		Ptr<Object> proto = array_prototype();
		proto->set_raw("new", gc_new<Function>(array_new));
		proto->set_raw("get", gc_new<Function>(array_get));
		proto->set_raw("set", gc_new<Function>(array_set));
		proto->set_raw("each", gc_new<Function>(array_each));
		proto->set_raw("each_parallel", gc_new<Function>(array_each_parallel));
		proto->set_raw("map", gc_new<Function>(array_map));
		proto->set_raw("map_parallel", gc_new<Function>(array_map_parallel));
		proto->set_raw("push", gc_new<Function>(array_push));		
		proto->set_raw("pop", gc_new<Function>(array_pop));
		proto->set_raw("unshift", gc_new<Function>(array_unshift));
		proto->set_raw("shift", gc_new<Function>(array_shift));
		proto->set_property_getter("length", gc_new<Function>(array_length));
		proto->set_raw("inspect", gc_new<Function>(array_inspect));
		proto->set_raw("to_string", proto->get_raw("inspect"));
		proto->set_raw("<<", gc_new<Function>(array_push));
	}
}

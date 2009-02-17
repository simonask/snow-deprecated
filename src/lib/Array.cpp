#include "Array.h"
#include "Runtime.h"

#define DEFAULT_ARRAY_LENGTH 8

namespace snow {
	static Handle<Object> ArrayPrototype = NULL;
	
	void Array::resize(size_t new_size) {
		// TODO: Grow more than strictly necessary to minimize allocations
		VALUE* old_pointer = m_Data;
		m_Data = (VALUE*)Garbage::alloc(new_size, true);
		m_AllocatedSize = new_size;
		if (m_Length != 0 && old_pointer) {
			memmove(m_Data, old_pointer, m_Length*sizeof(VALUE));
		}
		Garbage::unmark(old_pointer);
	}
	
	void Array::gc_mark() {
		// Avoid infinite recursion if we contain a reference to self
		if (!m_GCLock.lock())
			return;
		
		Object::gc_mark();
		Garbage::mark(m_Data);
		for (size_t i = 0; i < m_Length; ++i) {
			if (is_object(m_Data[i]))
				Garbage::mark(m_Data[i]);
		}
		
		m_GCLock.unlock();
	}
	
	void Array::gc_unmark() {
		// Avoid infinite recursion if we contain a reference to self
		if (!m_GCLock.lock())
			return;
		
		Object::gc_unmark();
		Garbage::unmark(m_Data);
		for (size_t i = 0; i < m_Length; ++i) {
			if (is_object(m_Data[i]))
				Garbage::unmark(m_Data[i]);
		}
		
		m_GCLock.unlock();
	}
	
	Array::Array(size_t preallocate_length) : Object(array_prototype()), m_Data(NULL), m_Length(0), m_AllocatedSize(0) {
		if (preallocate_length == 0)
			preallocate_length = DEFAULT_ARRAY_LENGTH;
		resize(preallocate_length * sizeof(VALUE));
	}
	
	Array::Array(const Array& other) : Object(array_prototype()), m_Data(other.m_Data), m_Length(other.m_Length), m_AllocatedSize(0) {
		resize(m_Length * sizeof(VALUE));
	}
	
	Array::~Array() {
		gc_unmark();
	}
	
	void Array::ensure_length(size_t len) {
		if (len*sizeof(VALUE) <= m_AllocatedSize)
			return;
		resize(len * sizeof(VALUE));
	}
	
	VALUE Array::get_by_index(int64_t idx) const {
		if (idx < 0)
			idx = m_Length + idx;
		if (idx < 0 || idx >= (int64_t)m_Length)
			return nil();
		return m_Data[idx];
	}
	
	VALUE Array::set_by_index(int64_t idx, VALUE val) {
		if (idx < 0)
			idx %= m_Length;
		ensure_length(idx+1);
		m_Data[idx] = val;
		if (idx >= (int64_t)m_Length)
			m_Length = idx+1;
		return val;
	}
	
	VALUE Array::push(VALUE val) {
		ensure_length(m_Length + 1);
		m_Data[m_Length] = val;
		++m_Length;
		return val;
	}
	
	VALUE Array::pop() {
		if (m_Length == 0)
			return nil();
		VALUE val = m_Data[m_Length-1];
		--m_Length;
		return val;
	}
	
	VALUE Array::unshift(VALUE val) {
		ensure_length(m_Length + 1);
		memmove(&m_Data[1], m_Data, m_Length);
		m_Data[0] = val;
		++m_Length;
		return val;
	}
	
	VALUE Array::shift() {
		if (m_Length == 0)
			return nil();
		VALUE val = m_Data[0];
		--m_Length;
		memmove(m_Data, &m_Data[1], m_Length);
		return val;
	}
	
	static VALUE array_get(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(num_args == 1);
		auto array = object_cast<Array>(self);
		int64_t idx = integer(args[0]);
		return array->get_by_index(idx);
	}
	
	static VALUE array_set(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(num_args == 2);
		auto array = object_cast<Array>(self);
		int idx = integer(args[0]);
		VALUE new_value = args[1];
		return array->set_by_index(idx, new_value);
	}
	
	static VALUE array_each(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(num_args >= 1);
		auto array = object_cast<Array>(self);
		
		VALUE closure = args[0];
		for (size_t i = 0; i < array->length(); ++i) {
			call(closure, 2, array->data()[i], value((int64_t)i));
		}
		return self;
	}
	
	static VALUE array_push(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(num_args == 1);
		auto array = object_cast<Array>(self);
		VALUE val = args[0];
		return array->push(val);
	}
	
	static VALUE array_pop(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_OBJECT(self, Array);
		ASSERT_ARGS(num_args == 0);
		auto array = object_cast<Array>(self);
		return array->pop();
	}
	
	Handle<Object>& array_prototype() {
		if (ArrayPrototype)
			return ArrayPrototype;
		
		Object* ap = new Object;
		ap->set("get", create_function(array_get));
		ap->set("set", create_function(array_set));
		ap->set("each", create_function(array_each));
		ap->set("push", create_function(array_push));
		ap->set("pop", create_function(array_pop));
/*		ap->set("unshift", create_function(array_unshift));
		ap->set("shift", create_function(array_shift));*/
		
		ArrayPrototype = ap;
		return ArrayPrototype;
	}
}
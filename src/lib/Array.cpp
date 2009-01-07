#include "Array.h"
#include "Runtime.h"

namespace snot {
	static Object* ArrayPrototype = NULL;
	
	Array::Array(const Array& other) : Object(array_prototype()), m_Values(other.m_Values.size()) {
		int i = 0;
		for (auto iter = iterate(other.m_Values); iter; ++iter, ++i) {
			m_Values[i] = snot::copy(*iter);
		}
	}
	
	static VALUE array_get(VALUE self, uint64_t num_args, VALUE* args) {
		assert_object(self, Array);
		assert_args(num_args == 1);
		auto array = object_cast<Array>(self);
		int idx = integer(args[0]);
		if (idx < 0)
			idx = array->values().size() + idx;
		if (idx < 0 || idx >= array->values().size())
			return undefined();
		return array->values()[idx];
	}
	
	static VALUE array_set(VALUE self, uint64_t num_args, VALUE* args) {
		assert_object(self, Array);
		assert_args(num_args == 2);
		auto array = object_cast<Array>(self);
		int idx = integer(args[0]);
		VALUE new_value = args[1];
		if (idx < 0)
			idx = array->values().size() + idx;
		
		snot::destroy(array->values()[idx]);		
		array->values()[idx] = snot::copy(new_value);
		
		return new_value;
	}
	
	static VALUE array_each(VALUE self, uint64_t num_args, VALUE* args) {
		assert_object(self, Array);
		assert_args(num_args >= 1);
		auto array = object_cast<Array>(self);
		
		VALUE closure = args[0];
		for (auto iter = iterate(array->values()); iter; ++iter) {
			call(closure, self, 1, *iter);
		}
		return self;
	}
	
	static VALUE array_each_with_index(VALUE self, uint64_t num_args, VALUE* args) {
		assert_object(self, Array);
		assert_args(num_args >= 1);
		auto array = object_cast<Array>(self);
		
		VALUE closure = args[0];
		int64_t i = 0;
		for (auto iter = iterate(array->values()); iter; ++iter, ++i) {
			call(closure, self, 2, *iter, value(i));
		}
		return self;
	}
	
	static VALUE array_push(VALUE self, uint64_t num_args, VALUE* args) {
		assert_object(self, Array);
		assert_args(num_args == 1);
		auto array = object_cast<Array>(self);
		
		VALUE val = args[0];
		array->values().push_back(snot::copy(val));
		return val;
	}
	
	static VALUE array_pop(VALUE self, uint64_t num_args, VALUE* args) {
		assert_object(self, Array);
		assert_args(num_args == 0);
		auto array = object_cast<Array>(self);
		VALUE popped = array->values().back();
		array->values().pop_back();
		return popped;
	}
	
	Object* array_prototype() {
		if (ArrayPrototype)
			return ArrayPrototype;
		
		Object* ap = new Object;
		ap->set("get", create_function(array_get));
		ap->set("set", create_function(array_set));
		ap->set("each", create_function(array_each));
		ap->set("each_with_index", create_function(array_each_with_index));
		ap->set("push", create_function(array_push));
		ap->set("pop", create_function(array_pop));
/*		ap->set("unshift", create_function(array_unshift));
		ap->set("shift", create_function(array_shift));*/
		
		ArrayPrototype = ap;
		return ap;
	}
}
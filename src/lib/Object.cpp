#include "Object.h"
#include "Integer.h"
#include "Undefined.h"
#include "Function.h"
#include "Runtime.h"

namespace snot {
	static Object* ObjectPrototype = NULL;
	
	VALUE Object::call(VALUE self, uint64_t num_args, ...) {
		va_list ap;
		va_start(ap, num_args);
		VALUE ret = va_call(self, num_args, ap);
		va_end(ap);
		return ret;
	}
	
	VALUE Object::va_call(VALUE self, uint64_t num_args, va_list& ap) {
		VALUE call_handler = get("call");
		if (is_object(call_handler))
			return object(call_handler)->va_call(self, num_args, ap);
		return value(this);
	}
	
	Object* Object::copy(bool deep) const {
		Object* obj = new Object;
		if (deep) {
			obj->m_Prototype = prototype();
			for (auto iter = iterate(m_Members); iter; ++iter) {
				obj->m_Members[iter->first] = snot::copy(iter->second, deep);
			}
		} else
			obj->m_Prototype = this;
		return obj;
	}
	
	VALUE Object::set(const char* member, VALUE value) {
		m_Members[member] = value;
		return value;
	}
	
	VALUE Object::get(const char* member) const {
		auto iter = m_Members.find(member);
		if (iter != m_Members.end())
			return iter->second;
		else {
			if (this != ObjectPrototype)
				return prototype()->get(member);
			else
				return undefined();
		}
	}
	
	static VALUE object_id(VALUE self, uint64_t num_args, VALUE* args) {
		assert(num_args == 0);
		return value(reinterpret_cast<int64_t>(self));
	}
	
	static VALUE object_send(VALUE self, uint64_t num_args, VALUE* args) {
		VALUE message = args[0];
		// TODO: convert message to string, send it, return the result
		return self;
	}
	
	static VALUE object_members(VALUE self, uint64_t num_args, VALUE* args) {
		// TODO: construct array of member names
		return self;
	}
	
	static VALUE object_get_prototype(VALUE self, uint64_t num_args, VALUE* args) {
		if (is_object(self))
			return value(object(self)->prototype());
		return value(object_for(self));
	}
	
	static VALUE object_copy(VALUE self, uint64_t num_args, VALUE* args) {
		bool deep = num_args > 0 && eval_truth(args);
		return copy(self, deep);
	}
	
	Object* object_prototype() {
		if (!ObjectPrototype) {
			ObjectPrototype = new Object;
			ObjectPrototype->set("object_id", create_function(object_id));
			VALUE send = create_function(object_send);
			ObjectPrototype->set("send", send);
			ObjectPrototype->set("__send__", send);
			ObjectPrototype->set("members", create_function(object_members));
			ObjectPrototype->set("prototype", create_function(object_get_prototype));
		}
		return ObjectPrototype;
	}
}
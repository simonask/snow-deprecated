#include "Object.h"
#include "Integer.h"
#include "Nil.h"
#include "Function.h"
#include "Runtime.h"

namespace snow {
	bool Object::has_member(VALUE member) const {
		auto iter = m_Members.find(member);
		return iter != m_Members.end();
	}
	
	VALUE Object::set(VALUE member, VALUE value) {
		m_Members[member] = value;
		return value;
	}
	
	VALUE Object::get(VALUE member) const {
		auto iter = m_Members.find(member);
		if (iter != m_Members.end()) {
			return iter->second;
		} else {
			if (this != prototype())
				return prototype()->get(member);
			else {
				debug("member `%s' not found on %llx", value_to_string(member), this);
				TRAP();
				return nil();
			}
		}
	}
	
	VALUE Object::va_call(VALUE self, uint64_t num_args, va_list& ap) {
		VALUE call_handler = get(symbol("__call__"));
		if (eval_truth(call_handler)) {
			return object_for(call_handler)->va_call(self, num_args, ap);
		}
		return self;
	}
	
	static VALUE object_id(VALUE self, uint64_t num_args, VALUE* args) {
		assert(num_args == 0);
		return value(reinterpret_cast<int64_t>(self));
	}
	
	static VALUE object_send(VALUE self, uint64_t num_args, VALUE* args) {
		//VALUE message = args[0];
		// TODO: convert message to string, send it, return the result
		return self;
	}
	
	static VALUE object_call(VALUE self, uint64_t num_args, VALUE* args) {
		Object* proto = object_prototype();
		if (num_args > 0)
		{
			proto = object_cast<Object>(args[0]);
			ASSERT(proto);
		}
		return new(kGarbage) Object(proto);
	}
	
	static VALUE object_members(VALUE self, uint64_t num_args, VALUE* args) {
		// TODO: construct array of member names
		return self;
	}
	
	static VALUE object_get_prototype(VALUE self, uint64_t num_args, VALUE* args) {
		if (is_object(self))
			return value(object_cast<Object>(self)->prototype());
		return value(object_for(self));
	}
	
	static VALUE object_to_string(VALUE self, uint64_t num_args, VALUE* args) {
		return create_string("Object");
	}
	
	static VALUE object_equals(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_ARGS(num_args == 1);
		return value(self == args[0]);
	}
	
	Object* object_prototype() {
		static Object* proto = NULL;
		if (proto) return proto;
		proto = new(kMalloc) Object;
		proto->set_by_string("name", create_string("Object"));
		proto->set_by_string("object_id", new Function(object_id));
		VALUE send = new Function(object_send);
		proto->set_by_string("send", send);
		proto->set_by_string("__send__", send);
		proto->set_by_string("__call__", new Function(object_call));
		proto->set_by_string("members", new Function(object_members));
		proto->set_by_string("prototype", new Function(object_get_prototype));
		proto->set_by_string("to_string", new Function(object_to_string));
		proto->set_by_string("=", new Function(object_equals));
		proto->set_prototype(proto);
		return proto;
	}
}

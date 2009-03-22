#include "Object.h"
#include "Integer.h"
#include "Nil.h"
#include "Function.h"
#include "Runtime.h"

namespace snow {
	bool Object::has_member(const std::string& member) const {
		auto iter = m_Members.find(member);
		return iter != m_Members.end();
	}
	
	VALUE Object::set(const std::string& member, VALUE value) {
		m_Members[member] = value;
		return value;
	}
	
	VALUE Object::get(const std::string& member) const {
		auto iter = m_Members.find(member);
		if (iter != m_Members.end()) {
			return iter->second;
		} else {
			if (this != prototype())
				return prototype()->get(member);
			else {
				debug("member `%s' not found on %llx", member.c_str(), this);
				TRAP();
				return nil();
			}
		}
	}
	
	VALUE Object::va_call(VALUE self, uint64_t num_args, va_list& ap) {
		VALUE call_handler = get("__call__");
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
		return new(kGarbage) Object;
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
		debug("object_to_string");
		return create_string("<Object>");
	}
	
	static VALUE object_equals(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_ARGS(num_args == 1);
		return value(self == args[0]);
	}
	
	Handle<Object>& object_prototype() {
		static Handle<Object> op;
		if (op) return op;
		op = new Object;
		op->set("name", create_string("Object"));
		op->set("object_id", new Function(object_id));
		VALUE send = new Function(object_send);
		op->set("send", send);
		op->set("__send__", send);
		op->set("__call__", new Function(object_call));
		op->set("members", new Function(object_members));
		op->set("prototype", new Function(object_get_prototype));
		op->set("to_string", new Function(object_to_string));
		op->set("=", new Function(object_equals));
		op->set_prototype(op);
		return op;
	}
}
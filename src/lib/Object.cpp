#include "Object.h"
#include "Integer.h"
#include "Nil.h"
#include "Function.h"
#include "Runtime.h"
#include "InternalMacros.h"

namespace snow {
	static Handle<Object> ObjectPrototype = NULL;
	
	VALUE Object::call(const ValueHandle& self, uint64_t num_args, ...) {
		va_list ap;
		va_start(ap, num_args);
		VALUE ret = va_call(self, num_args, ap);
		va_end(ap);
		return ret;
	}
	
	VALUE Object::va_call(const ValueHandle& self, uint64_t num_args, va_list& ap) {
		return value(this);
	}
	
	VALUE Object::set(const char* member, VALUE value) {
		m_Members[member] = value;
		return value;
	}
	
	VALUE Object::get(const char* member) const {
		auto iter = m_Members.find(std::string(member));
		if (iter != m_Members.end()) {
			return iter->second;
		} else {
			if (this != prototype())
				return prototype()->get(member);
			else
				return nil();
		}
	}
	
	static VALUE object_id(Scope* scope) {
		assert(NUM_ARGS == 0);
		return value(reinterpret_cast<int64_t>(SELF.value()));
	}
	
	static VALUE object_send(Scope* scope) {
		//VALUE message = args[0];
		// TODO: convert message to string, send it, return the result
		return SELF;
	}
	
	static VALUE object_call(Scope* scope) {
		return SELF;
	}
	
	static VALUE object_members(Scope* scope) {
		// TODO: construct array of member names
		return SELF;
	}
	
	static VALUE object_get_prototype(Scope* scope) {
		if (SELF.is_object())
			return value(SELF.cast<Object>()->prototype());
		return value(object_for(SELF));
	}
	
	static VALUE object_copy(Scope* scope) {
		return copy(SELF);
	}
	
	static VALUE object_to_string(Scope* scope) {
		debug("object_to_string");
		return create_string("<Object>");
	}
	
	static VALUE object_equals(Scope* scope) {
		ASSERT_ARGS(scope->arguments()->length() == 1);
		return value(scope->self() == scope->arguments()->get_by_index(0));
	}
	
	Handle<Object>& object_prototype() {
		if (!ObjectPrototype) {
			Handle<Object> op = new Object;
			op->set("name", create_string("Object"));
			op->set("object_id", create_function(object_id));
			op->set("copy", create_function(object_copy));
			VALUE send = create_function(object_send);
			op->set("send", send);
			op->set("__send__", send);
			op->set("__call__", create_function(object_call));
			op->set("members", create_function(object_members));
			op->set("prototype", create_function(object_get_prototype));
			op->set("to_string", create_function(object_to_string));
			op->set("=", create_function(object_equals));
			op->set_prototype(op);
			ObjectPrototype = op;
		}
		return ObjectPrototype;
	}
}
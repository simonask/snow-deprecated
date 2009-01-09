#include "Object.h"
#include "Integer.h"
#include "Undefined.h"
#include "Function.h"
#include "Runtime.h"

namespace snow {
	static Object* ObjectPrototype = NULL;
	
	Object::~Object() {
		if (m_Members.ref_count() == 1) {
			for (auto iter = iterate(*m_Members); iter; ++iter) {
				if (is_object(iter->second))
					delete object(iter->second);
			}
		}
	}
	
	VALUE Object::call(VALUE self, uint64_t num_args, ...) {
		va_list ap;
		va_start(ap, num_args);
		VALUE ret = va_call(self, num_args, ap);
		va_end(ap);
		return ret;
	}
	
	VALUE Object::va_call(VALUE self, uint64_t num_args, va_list& ap) {
		VALUE call_handler = get("__call__");
		if (is_object(call_handler))
			return object(call_handler)->va_call(self, num_args, ap);
		// If there is no call handler, this is not a functor, so just return this
		return value(this);
	}
	
	VALUE Object::set(const char* member, VALUE value) {
		if (m_Members.ref_count() != 1) {
			RefPtr<Members> new_members = new Members;
			for (auto iter = iterate(*m_Members); iter; ++iter) {
				VALUE val;
				if (is_object(iter->second))
					val = object(iter->second)->copy();
				else
					val = iter->second;
				(*new_members)[iter->first] = val;
			}
			m_Members = new_members;
		}
		(*m_Members)[member] = value;
		return value;
	}
	
	VALUE Object::get(const char* member) const {
		auto iter = m_Members->find(member);
		if (iter != m_Members->end())
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
		//VALUE message = args[0];
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
		return copy(self);
	}
	
	Object* object_prototype() {
		if (!ObjectPrototype) {
			ObjectPrototype = new Object;
			ObjectPrototype->set("object_id", create_function(object_id));
			ObjectPrototype->set("copy", create_function(object_copy));
			VALUE send = create_function(object_send);
			ObjectPrototype->set("send", send);
			ObjectPrototype->set("__send__", send);
			ObjectPrototype->set("members", create_function(object_members));
			ObjectPrototype->set("prototype", create_function(object_get_prototype));
		}
		return ObjectPrototype;
	}
}
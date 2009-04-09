#include "Object.h"
#include "Integer.h"
#include "Nil.h"
#include "Function.h"
#include "Runtime.h"

namespace snow {
	GC_ROOTS_IMPL(Object) {
		GC_SUPER(ThinObject);

		for each (iter, m_Members) {
			// Don't need to mark the keys, since they're always symbols (hopefully)
			GC_ROOT(iter->second);
		}

		for each (iter, m_Properties) {
			// ditto for the keys
			GC_ROOT(iter->second.getter);
			GC_ROOT(iter->second.setter);
		}
	}

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
				return nil();
			}
		}
	}

	VALUE Object::set_with_property(VALUE self, VALUE member, VALUE val) {
		auto prop = property(member);
		if (prop) {
			if (eval_truth(prop->setter))
				return snow::call(self, prop->setter, 1, val);
			// TODO: Exception
			error("non-settable property `%s'", value_to_string(member));
			TRAP();
		}

		return set(member, val);
	}

	VALUE Object::get_with_property(VALUE self, VALUE member) const {
		auto prop = property(member);
		if (prop) {
			if (eval_truth(prop->getter))
				return snow::call(self, prop->getter, 0);
			// TODO: Exception
			error("non-gettable property `%s'", value_to_string(member));
			TRAP();
		}

		return get(member);
	}
	
	VALUE Object::va_call(VALUE self, uint64_t num_args, va_list& ap) {
		// TODO: Functors
		return self;
	}

	const Object::PropertyPair* Object::property(VALUE name) const {
		ASSERT(is_symbol(name));
		auto iter = m_Properties.find(name);
		if (iter == m_Properties.end()) {
			if (this != prototype())
				return prototype()->property(name);
			return NULL;
		}
		else
			return &iter->second;
	}

	void Object::set_property(VALUE name, VALUE getter, VALUE setter) {
		ASSERT(is_symbol(name));
		m_Properties[name] = PropertyPair(getter, setter);
	}

	void Object::set_property_getter(VALUE name, VALUE getter) {
		ASSERT(is_symbol(name));
		auto iter = m_Properties.find(name);
		if (iter == m_Properties.end())
			m_Properties[name] = PropertyPair(getter, NULL);
		else
			iter->second.getter = getter;
	}

	void Object::set_property_setter(VALUE name, VALUE setter) {
		ASSERT(is_symbol(name));
		auto iter = m_Properties.find(name);
		if (iter == m_Properties.end())
			m_Properties[name] = PropertyPair(NULL, setter);
		else
			iter->second.setter = setter;
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
	
	static VALUE object_new(VALUE self, uint64_t num_args, VALUE* args) {
		Object* proto = object_prototype();
		if (num_args > 0)
		{
			proto = object_cast<Object>(args[0]);
			ASSERT(proto);
		}
		return new(kGarbage) Object(proto);
	}

	static VALUE object_copy(VALUE self, uint64_t num_args, VALUE* args) {
		if (is_object(self))
			return object_cast<IObject>(self)->copy();
		else
			return self;
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

	static VALUE object_property(VALUE self, uint64_t num_args, VALUE* args) {
		ASSERT_ARGS(num_args >= 2);
		Object* object = object_cast<Object>(self);
		ASSERT(object);
		VALUE name = args[0];
		VALUE getter = args[1];
		VALUE setter = NULL;
		if (num_args >= 3) {
			setter = args[2];
		}

		object->set_property(name, getter, setter);
		return self;
	}
	
	Object* object_prototype() {
		static Object* proto = NULL;
		if (proto) return proto;
		proto = new(kMalloc) Object;
		proto->set_by_string("name", create_string("Object"));
		proto->set_by_string("object_id", new Function(object_id));
		proto->set_by_string("new", new Function(object_new));
		proto->set_by_string("copy", new Function(object_copy));
		proto->set_by_string("members", new Function(object_members));
		proto->set_by_string("prototype", new Function(object_get_prototype));
		proto->set_by_string("to_string", new Function(object_to_string));
		proto->set_by_string("=", new Function(object_equals));
		proto->set_by_string("property", new Function(object_property));
		proto->set_prototype(proto);
		return proto;
	}
}

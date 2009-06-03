#include "Object.h"
#include "Integer.h"
#include "Nil.h"
#include "Function.h"
#include "Runtime.h"
#include "SnowString.h"
#include "Exception.h"
#include "Array.h"

namespace snow {
	GC_ROOTS_IMPL(Object) {
		GC_SUPER(ThinObject);

		GC_NONPOINTER_ROOT(m_Members);

		gc_property_root_node(_gc, _op, m_Properties.root());
	}
	
	void Object::gc_property_root_node(IGarbageCollector& _gc, IGarbageCollector::GCOperation _op, Object::Properties::Node*& node) {
		if (!node) return;
		GC_ROOT(node);
		GC_ROOT(node->parent);
		//GC_ROOT(node->key); // key must be a Symbol, so let's not waste time trying
		GC_ROOT(node->value.getter);
		GC_ROOT(node->value.setter);
		gc_property_root_node(_gc, _op, node->left);
		gc_property_root_node(_gc, _op, node->right);
	}

	VALUE Object::set_raw(VALUE member, VALUE value) {
		m_Members[member] = value;
		return value;
	}
	
	VALUE Object::get_raw(VALUE member) const {
		auto value = m_Members.find(member);
		if (!value && this != prototype()) {
			value = prototype()->get_raw(member);
		}
		return value;
	}

	VALUE Object::set(VALUE self, VALUE member, VALUE val) {
		auto prop = property(member);
		if (prop) {
			if (eval_truth(prop->setter))
				return snow::call(self, prop->setter, 1, val);
			throw_exception(new String("Trying to set non-writable property `%'.", member));
		}

		// self might be different from this, if we're a prototype
		auto self_obj = object_cast<IObject>(self);
		if (!self_obj)
			throw_exception(new String("Trying to set non-property member of immediate: %", self));

		return self_obj->set_raw(member, val);
	}

	VALUE Object::get(VALUE self, VALUE member) const {
		HandleScope _;
		auto prop = property(member);
		if (prop) {
			if (eval_truth(prop->getter))
				return snow::call(self, prop->getter, 0);
			throw_exception(new String("Trying to get non-readable property `%'.", member));
		}

		// self might be different from this, if we're a prototype
		Handle<IObject> self_obj = object_for(self);

		ValueHandle val = self_obj->get_raw(member);
		if (val) {
			return val;
		} else {
			static const VALUE member_missing_symbol = symbol("member_missing");
			ValueHandle member_missing_handler(self_obj->get_raw(member_missing_symbol));
			if (member_missing_handler)
				return snow::call(self, member_missing_handler, 1, member);
			else {
				throw_exception(new String("No member `%', and no member_missing handler.", member));
				return NULL;
			}
		}
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
			return &iter->value;
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
			iter->value.getter = getter;
	}

	void Object::set_property_setter(VALUE name, VALUE setter) {
		ASSERT(is_symbol(name));
		auto iter = m_Properties.find(name);
		if (iter == m_Properties.end())
			m_Properties[name] = PropertyPair(NULL, setter);
		else
			iter->value.setter = setter;
	}
	
	static VALUE object_id(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		auto object = object_cast<IObject>(self);
		uint64_t the_id;
		if (object) {
			the_id = object->id();
			// Avoid collision with immediates
			the_id <<= 4;
		}
		else
			the_id = reinterpret_cast<uint64_t>(self);
		return value(static_cast<int64_t>(the_id));
	}
	
	static VALUE object_send(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		//VALUE message = args[0];
		// TODO: convert message to string, send it, return the result
		return self;
	}
	
	static VALUE object_new(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		Object* proto = object_prototype();
		if (num_args > 0)
		{
			proto = object_cast<Object>(args[0]);
			ASSERT(proto);
		}
		return new(kGarbage) Object(proto);
	}

	static VALUE object_call(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		return self;
	}

	static VALUE object_copy(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		if (is_object(self))
			return object_cast<IObject>(self)->copy();
		else
			return self;
	}
	
	static VALUE object_members(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		auto object = object_cast<Object>(self);
		if (object) {
			Handle<Array> result = new Array();
			result->preallocate(object->members().size());
			for each (iter, object->members()) {
				result->push(iter->key);
			}
			return result;
		}
		// Immediates and thin objects
		return new Array();
	}
	
	static VALUE object_get_prototype(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		if (is_object(self))
			return object_cast<IObject>(self)->prototype();
		return object_for(self);
	}
	
	static VALUE object_to_string(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		return create_string("Object");
	}
	
	static VALUE object_equals(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(num_args == 1);
		return value(self == args[0]);
	}

	static VALUE object_property(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
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

	static VALUE object_member_missing(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		throw_exception(new String("No such member: `%'", args[0]));
		return nil();
	}
	
	Object* object_prototype() {
		static Object* proto = NULL;
		if (proto) return proto;
		proto = new(kMalloc) Object;
		proto->set_raw_s("name", create_string("Object"));
		proto->set_property(symbol("object_id"), new Function(object_id), NULL);
		proto->set_property(symbol("members"), new Function(object_members), NULL);
		proto->set_property(symbol("prototype"), new Function(object_get_prototype), NULL);
		proto->set_raw_s("__call__", new Function(object_call));
		proto->set_raw_s("new", new Function(object_new));
		proto->set_raw_s("copy", new Function(object_copy));
		proto->set_raw_s("to_string", new Function(object_to_string));
		proto->set_raw_s("=", new Function(object_equals));
		proto->set_raw_s("property", new Function(object_property));
		proto->set_raw_s("member_missing", new Function(object_member_missing));
		proto->set_prototype(proto);
		return proto;
	}
}

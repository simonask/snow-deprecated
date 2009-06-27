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

	Value Object::set_raw(Symbol member, const Value& value) {
		m_Members[member] = value.value();
		return value;
	}
	
	Value Object::get_raw(Symbol member) const {
		Value value = m_Members.find(member);
		if (!value && prototype() != this) {
			value = prototype()->get_raw(member);
		}
		return value;
	}

	Value Object::set(const Value& self, Symbol member, const Value& val) {
		auto prop = property(member);
		if (prop) {
			if (eval_truth(prop->setter.value()))
				return snow::call(self, prop->setter, val);
			throw_exception(gc_new<String>("Trying to set non-writable property `%'.", member));
		}

		// self might be different from this, if we're a prototype
		auto self_obj = object_cast<IObject>(self);
		if (!self_obj)
			throw_exception(gc_new<String>("Trying to set non-property member of immediate: %", self));

		return self_obj->set_raw(member, val);
	}

	Value Object::get(const Value& self, Symbol member) const {
		HandleScope _;
		auto prop = property(member);
		if (prop) {
			if (eval_truth(prop->getter.value()))
				return snow::call(self, prop->getter);
			throw_exception(gc_new<String>("Trying to get non-readable property `%'.", member));
		}

		// self might be different from this, if we're a prototype
		Handle<IObject> self_obj = object_for(self);

		ValueHandle val = self_obj->get_raw(member);
		if (val) {
			return val;
		} else {
			static const Symbol member_missing_symbol("member_missing");
			ValueHandle member_missing_handler(self_obj->get_raw(member_missing_symbol));
			if (member_missing_handler)
				return snow::call(self, member_missing_handler, member);
			else {
				throw_exception(gc_new<String>("No member `%', and no member_missing handler.", member));
				return NULL;
			}
		}
	}
	
	const Object::PropertyPair* Object::property(Symbol name) const {
		ASSERT(is_symbol(name));
		auto iter = m_Properties.find(name);
		if (iter == m_Properties.end()) {
			if (prototype() != this)
				return prototype()->property(name);
			return NULL;
		}
		else
			return &iter->value;
	}

	void Object::set_property(Symbol name, const Value& getter, const Value& setter) {
		ASSERT(is_symbol(name));
		m_Properties[name] = PropertyPair(getter, setter);
	}

	void Object::set_property_getter(Symbol name, const Value& getter) {
		ASSERT(is_symbol(name));
		auto iter = m_Properties.find(name);
		if (iter == m_Properties.end())
			m_Properties[name] = PropertyPair(getter, NULL);
		else
			iter->value.getter = getter;
	}

	void Object::set_property_setter(Symbol name, const Value& setter) {
		ASSERT(is_symbol(name));
		auto iter = m_Properties.find(name);
		if (iter == m_Properties.end())
			m_Properties[name] = PropertyPair(NULL, setter);
		else
			iter->value.setter = setter;
	}
	
	static Value object_id(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		auto object = object_cast<IObject>(self);
		uintx the_id;
		if (object) {
			the_id = object->id();
			// Avoid collision with immediates
			the_id <<= 4;
		}
		else
			the_id = reinterpret_cast<uintx>(self.value());
		return value(static_cast<intx>(the_id));
	}
	
	static Value object_send(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		//VALUE message = args.data[0];
		// TODO: convert message to string, send it, return the result
		return self;
	}
	
	static Value object_new(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		Ptr<Object> proto = object_prototype();
		if (args.size > 0)
		{
			proto = object_cast<Object>(args.data[0]);
			ASSERT(proto);
		}
		return gc_new<Object>(proto);
	}

	static Value object_call(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		return self;
	}

	static Value object_copy(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		if (self.is_object())
			return object_cast<IObject>(self)->copy();
		else
			return self;
	}
	
	static Value object_members(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		auto object = object_cast<Object>(self);
		if (object) {
			Handle<Array> result = gc_new<Array>();
			result->preallocate(object->members().size());
			for each (iter, object->members()) {
				result->push(iter->key);
			}
			return result;
		}
		// Immediates and thin objects
		return gc_new<Array>();
	}
	
	static Value object_get_prototype(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		if (self.is_object())
			return object_cast<IObject>(self)->prototype();
		return object_for(self);
	}
	
	static Value object_to_string(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		return gc_new<String>("Object");
	}
	
	static Value object_equals(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(args.size == 1);
		return value(self == args.data[0]);
	}

	static Value object_property(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		ASSERT_ARGS(args.size >= 2);
		Ptr<Object> object = object_cast<Object>(self);
		ASSERT(object);
		VALUE name = args.data[0];
		VALUE getter = args.data[1];
		VALUE setter = NULL;
		if (args.size >= 3) {
			setter = args.data[2];
		}

		object->set_property(name, getter, setter);
		return self;
	}

	static Value object_member_missing(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		throw_exception(gc_new<String>("No such member: `%'", args.data[0]));
		return nil();
	}
	
	Ptr<Object> object_prototype() {
		static Ptr<Object> proto;
		if (proto) return proto;
		proto = gc_new<Object>();
		proto->set_raw("name", gc_new<String>("Object"));
		proto->set_property("object_id", gc_new<Function>(object_id), NULL);
		proto->set_property("members", gc_new<Function>(object_members), NULL);
		proto->set_property("prototype", gc_new<Function>(object_get_prototype), NULL);
		proto->set_raw("__call__", gc_new<Function>(object_call));
		proto->set_raw("new", gc_new<Function>(object_new));
		proto->set_raw("copy", gc_new<Function>(object_copy));
		proto->set_raw("to_string", gc_new<Function>(object_to_string));
		proto->set_raw("=", gc_new<Function>(object_equals));
		proto->set_raw("property", gc_new<Function>(object_property));
		proto->set_raw("member_missing", gc_new<Function>(object_member_missing));
		proto->set_prototype(proto);
		return proto;
	}
}

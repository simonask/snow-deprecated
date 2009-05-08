#include "Hash.h"
#include "SnowString.h"
#include "Function.h"
#include "Array.h"

namespace snow {
	GC_ROOTS_IMPL(Hash) {
		GC_SUPER(Object);
		GC_NONPOINTER_ROOT(m_Map);
	}

	VALUE Hash::va_call(VALUE self, uint64_t num_args, va_list& ap) {
		ASSERT_ARGS(num_args == 1);
		VALUE key = va_arg(ap, VALUE);
		return get_by_key(key);
	}

	Array* Hash::keys() const {
		Array* ar = new Array(size());
		size_t i = 0;
		for each (iter, m_Map) {
			(*ar)[i++] = iter->key;
		}
		return ar;
	}

	Array* Hash::values() const {
		Array* ar = new Array(size());
		size_t i = 0;
		for each (iter, m_Map) {
			(*ar)[i++] = iter->value;
		}
		return ar;
	}

	/// Snow API functions

	static VALUE hash_new(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		return new Hash();
	}

	static VALUE hash_get(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		auto hash = object_cast<Hash>(self);
		ASSERT(hash);
		ASSERT_ARGS(num_args > 0);
		return hash->get_by_key(args[0]);
	}

	static VALUE hash_set(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		auto hash = object_cast<Hash>(self);
		ASSERT(hash);
		ASSERT_ARGS(num_args == 2);
		return hash->set_by_key(args[0], args[1]);
	}

	static VALUE hash_delete(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		auto hash = object_cast<Hash>(self);
		ASSERT(hash);
		ASSERT_ARGS(num_args == 1);
		return hash->erase_by_key(args[0]);
	}

	static VALUE hash_length(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		auto hash = object_cast<Hash>(self);
		ASSERT(hash);
		ASSERT_ARGS(num_args == 0);
		return value((int64_t)hash->length());
	}

	static VALUE hash_keys(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		auto hash = object_cast<Hash>(self);
		ASSERT(hash);
		ASSERT_ARGS(num_args == 0);
		return hash->keys();
	}

	static VALUE hash_values(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		auto hash = object_cast<Hash>(self);
		ASSERT(hash);
		ASSERT_ARGS(num_args == 0);
		return hash->values();
	}

	static VALUE hash_inspect(VALUE self, uint64_t num_args, VALUE* args) {
		NORMAL_SCOPE();
		auto hash = object_cast<Hash>(self);
		ASSERT(hash);
		ASSERT_ARGS(num_args == 0);

		// XXX: Convert to Array because it is currently unsafe to iterate through a
		// ValueMap if a GC happens while iterating.
		Handle<Array> keys = hash->keys();
		Handle<Array> values = hash->values();

		std::stringstream ss;
		ss << "hash(";
		size_t len = keys->length();
		for (size_t i = 0; i < len; ++i) {
			ss << value_to_string(snow::call_method((*keys)[i], "inspect", 0));
			ss << " => ";
			ss << value_to_string(snow::call_method((*values)[i], "inspect", 0));
			if (i != len-1) {
				ss << ", ";
			}
		}
		ss << ")";
		return new String(ss.str());
	}

	Object* hash_prototype() {
		static Object* proto = NULL;
		if (proto) return proto;
		proto = new(kMalloc) Object;
		proto->set_raw_s("new", new Function(hash_new));
		proto->set_raw_s("name", new String("Hash"));
		proto->set_raw_s("get", new Function(hash_get));
		proto->set_raw_s("set", new Function(hash_set));
		proto->set_raw_s("erase", new Function(hash_delete));
		proto->set_raw_s("delete", proto->get_raw_s("erase"));
		proto->set_raw_s("inspect", new Function(hash_inspect));
		proto->set_property_getter(symbol("length"), new Function(hash_length));
		proto->set_property_getter(symbol("keys"), new Function(hash_keys));
		proto->set_property_getter(symbol("values"), new Function(hash_values));
		return proto;
	}
}



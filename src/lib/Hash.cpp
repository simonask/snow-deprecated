#include "Hash.h"
#include "SnowString.h"
#include "Function.h"
#include "Array.h"

namespace snow {
	GC_ROOTS_IMPL(Hash) {
		GC_SUPER(Object);
		for each (iter, m_Map) {
			//GC_ROOT(iter->first);
			GC_ROOT(iter->second);
		}
	}

	VALUE Hash::va_call(VALUE self, uint64_t num_args, va_list& ap) {
		ASSERT_ARGS(num_args == 1);
		VALUE key = va_arg(ap, VALUE);
		return get_by_key(key);
	}

	VALUE Hash::get_by_key(VALUE key) const {
		auto iter = m_Map.find(key);
		if (iter == m_Map.end()) {
			return nil();
		} else {
			return iter->second;
		}
	}

	VALUE Hash::set_by_key(VALUE key, VALUE val) {
		m_Map[key] = val;
		return val;
	}

	size_t Hash::length() const {
		return m_Map.size();
	}

	Array* Hash::keys() const {
		Array* ar = new Array(length());
		size_t i = 0;
		for each (iter, m_Map) {
			(*ar)[i++] = iter->first;
		}
		return ar;
	}

	Array* Hash::values() const {
		Array* ar = new Array(length());
		size_t i = 0;
		for each (iter, m_Map) {
			(*ar)[i++] = iter->second;
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

	Object* hash_prototype() {
		static Object* proto = NULL;
		if (proto) return proto;
		proto = new(kMalloc) Object;
		proto->set_raw_s("new", new Function(hash_new));
		proto->set_raw_s("name", new String("Hash"));
		proto->set_raw_s("get", new Function(hash_get));
		proto->set_raw_s("set", new Function(hash_set));
		proto->set_property_getter(symbol("length"), new Function(hash_length));
		proto->set_property_getter(symbol("keys"), new Function(hash_keys));
		proto->set_property_getter(symbol("values"), new Function(hash_values));
		return proto;
	}
}



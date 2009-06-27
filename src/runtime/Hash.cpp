#include "Hash.h"
#include "SnowString.h"
#include "Function.h"
#include "Array.h"

namespace snow {
	GC_ROOTS_IMPL(Hash) {
		GC_SUPER(Object);
		GC_NONPOINTER_ROOT(m_Map);
	}

	Value Hash::call(const Value& self, const Arguments& args) {
		ASSERT_ARGS(args.size >= 1);
		return get_by_key(args.data[0]);
	}

	Ptr<Array> Hash::keys() const {
		Ptr<Array> ar = gc_new<Array>();
		ar->preallocate(size());
		size_t i = 0;
		for each (iter, m_Map) {
			(*ar)[i++] = iter->key;
		}
		return ar;
	}

	Ptr<Array> Hash::values() const {
		Ptr<Array> ar = gc_new<Array>();
		ar->preallocate(size());
		size_t i = 0;
		for each (iter, m_Map) {
			(*ar)[i++] = iter->value;
		}
		return ar;
	}

	/// Snow API functions

	static Value hash_new(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		return gc_new<Hash>();
	}

	static Value hash_get(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		auto hash = object_cast<Hash>(self);
		ASSERT(hash);
		ASSERT_ARGS(args.size > 0);
		return hash->get_by_key(args.data[0]);
	}

	static Value hash_set(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		auto hash = object_cast<Hash>(self);
		ASSERT(hash);
		ASSERT_ARGS(args.size == 2);
		return hash->set_by_key(args.data[0], args.data[1]);
	}

	static Value hash_delete(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		auto hash = object_cast<Hash>(self);
		ASSERT(hash);
		ASSERT_ARGS(args.size == 1);
		return hash->erase_by_key(args.data[0]);
	}

	static Value hash_length(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		auto hash = object_cast<Hash>(self);
		ASSERT(hash);
		ASSERT_ARGS(args.size == 0);
		return value((intx)hash->length());
	}

	static Value hash_keys(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		auto hash = object_cast<Hash>(self);
		ASSERT(hash);
		ASSERT_ARGS(args.size == 0);
		return hash->keys();
	}

	static Value hash_values(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		auto hash = object_cast<Hash>(self);
		ASSERT(hash);
		ASSERT_ARGS(args.size == 0);
		return hash->values();
	}

	static Value hash_inspect(const Value& self, const Arguments& args) {
		NORMAL_SCOPE();
		auto hash = object_cast<Hash>(self);
		ASSERT(hash);
		ASSERT_ARGS(args.size == 0);

		// XXX: Convert to Array because it is currently unsafe to iterate through a
		// ValueMap if a GC happens while iterating.
		Handle<Array> keys = hash->keys();
		Handle<Array> values = hash->values();

		std::stringstream ss;
		ss << "hash(";
		size_t len = keys->length();
		for (size_t i = 0; i < len; ++i) {
			ss << value_to_string(snow::call_method((*keys)[i], "inspect"));
			ss << " => ";
			ss << value_to_string(snow::call_method((*values)[i], "inspect"));
			if (i != len-1) {
				ss << ", ";
			}
		}
		ss << ")";
		return gc_new<String>(ss.str());
	}

	Ptr<Object> hash_prototype() {
		static Ptr<Object> proto;
		if (proto) return proto;
		proto = malloc_new<Object>();
		proto->set_raw("new", gc_new<Function>(hash_new));
		proto->set_raw("name", gc_new<String>("Hash"));
		proto->set_raw("get", gc_new<Function>(hash_get));
		proto->set_raw("set", gc_new<Function>(hash_set));
		proto->set_raw("erase", gc_new<Function>(hash_delete));
		proto->set_raw("delete", proto->get_raw("erase"));
		proto->set_raw("inspect", gc_new<Function>(hash_inspect));
		proto->set_property_getter("length", gc_new<Function>(hash_length));
		proto->set_property_getter("keys", gc_new<Function>(hash_keys));
		proto->set_property_getter("values", gc_new<Function>(hash_values));
		return proto;
	}
}



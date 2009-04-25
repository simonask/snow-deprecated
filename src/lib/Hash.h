#ifndef HASH_H_73HJAPO
#define HASH_H_73HJAPO

#include "Object.h"

namespace snow {
	class Array;

	Object* hash_prototype();

	class Hash : public Object {
	private:
		GC_ROOTS;

		std::unordered_map<VALUE, VALUE> m_Map;
	public:
		Hash();
		Hash(const Hash& other);
		~Hash();
		IObject* copy() const { return new Hash(*this); }
		VALUE va_call(VALUE self, uint64_t num_args, va_list&);

		VALUE get_by_key(VALUE key) const;
		VALUE set_by_key(VALUE key, VALUE val);

		size_t length() const;
		Array* keys() const;
		Array* values() const;

		VALUE operator[](VALUE val) const { return get_by_key(val); }
	};


	inline Hash::Hash() : Object(hash_prototype()) { }
	inline Hash::Hash(const Hash& other) : Object(hash_prototype()), m_Map(other.m_Map) {	}
	inline Hash::~Hash() { }
}

#endif // HASH_H_73HJAPO


#ifndef HASH_H_73HJAPO
#define HASH_H_73HJAPO

#include "Object.h"

namespace snow {
	class Array;

	Ptr<Object> hash_prototype();

	class Hash : public Object {
	private:
		GC_ROOTS;

		ObjectMap m_Map;
	public:
		NO_INIT;

		Hash();
		Hash(const Hash& other);
		~Hash();
		Ptr<IObject> copy() const { return new Hash(*this); }
		Value call(const Value& self, const Arguments&);

		VALUE get_by_key(VALUE key) const { return m_Map.find(key); }
		VALUE set_by_key(VALUE key, VALUE val) { m_Map[key] = val; return val; }
		VALUE erase_by_key(VALUE key) { return value(m_Map.erase(key)); }

		size_t size() const { return m_Map.size(); }
		size_t length() const { return size(); }
		Ptr<Array> keys() const;
		Ptr<Array> values() const;

		VALUE& operator[](VALUE key) { return m_Map[key]; }
		VALUE operator[](VALUE key) const { return m_Map.find(key); }
	};


	inline Hash::Hash() : Object(hash_prototype()) { }
	inline Hash::Hash(const Hash& other) : Object(hash_prototype()), m_Map(other.m_Map) {	}
	inline Hash::~Hash() { }
}

#endif // HASH_H_73HJAPO


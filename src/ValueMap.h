#ifndef VALUEMAP_H_QBVHSP4
#define VALUEMAP_H_QBVHSP4

#include "lib/Value.h"
#include "lib/Runtime.h"
#include "IGarbage.h"
#include "RBTree.h"

namespace snow {
	// ValueMap is a simple (and naïve) implementation of a hash map. It
	// cannot be used directly in Snow, but is used by classes such as Object
	// and Hash to provide a well-performing mapping between VALUEs.
	class ValueMap : public IGarbage {
	public:
		class ValueComparator { public: int operator()(VALUE a, VALUE b) const; };
		class NodeAllocator {
		public:
			void* alloc(size_t);
			inline void free(void*) {};
		};

		typedef RBTree<VALUE, VALUE, ValueComparator, NodeAllocator> InternalTree;
	private:
		GC_ROOTS;
		
		// ValueMap pointers may not be casted to VALUE, so there is no reason to implement gc locking.
		inline bool gc_try_lock() { return true; }
		inline void gc_unlock() {}

		InternalTree m_Tree;
	public:
		ValueMap() {}
		~ValueMap() {}

		InternalTree::Iterator begin() { return m_Tree.begin(); }
		InternalTree::Iterator end() { return m_Tree.end(); }
		InternalTree::ConstIterator begin() const { return m_Tree.begin(); }
		InternalTree::ConstIterator end() const { return m_Tree.end(); }

		// Returns NULL if key is not found, otherwise the value.
		VALUE find(VALUE key) const;

		void insert(VALUE key, VALUE val) { m_Tree.insert(key, val); }

		// Returns NULL if the key is not found, otherwise the value.
		VALUE erase(VALUE key);

		size_t size() const { return m_Tree.size(); }
	
		VALUE& operator[](VALUE key) { return m_Tree[key]; }
	};

	inline int ValueMap::ValueComparator::operator()(VALUE _a, VALUE _b) const {
		uint64_t a = snow::get_object_id(_a);
		uint64_t b = snow::get_object_id(_b);
		return a < b ? -1 : (a > b ? 1 : 0);
	}

	inline void* ValueMap::NodeAllocator::alloc(size_t sz) {
		return new(kGarbage, kBlob) byte[sz];
	}

	inline VALUE ValueMap::find(VALUE key) const {
		InternalTree::ConstIterator iter = m_Tree.find(key);
		if (iter != m_Tree.end())
			return iter->value;
		return NULL;
	}

	inline VALUE ValueMap::erase(VALUE key) {
		VALUE erased = NULL;
		m_Tree.erase(key, &erased);
		return erased;
	}
}

#endif // VALUEMAP_H_QBVHSP4


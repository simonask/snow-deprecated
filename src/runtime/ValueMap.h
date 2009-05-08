#ifndef VALUEMAP_H_QBVHSP4
#define VALUEMAP_H_QBVHSP4

#include "runtime/Value.h"
#include "runtime/Runtime.h"
#include "gc/IGarbage.h"
#include "base/RBTree.h"

namespace snow {
	class ImmediateComparator {
	public:
		int operator()(VALUE a, VALUE b) const;
	};

	class ObjectComparator {
	public:
		int operator()(VALUE a, VALUE b) const;
	};

	class GarbageAllocatorProxy {
	public:
		void* alloc(size_t);
		inline void free(void*) {};
	};

	// ValueMap is a simple (and naïve) implementation of a hash map. It
	// cannot be used directly in Snow, but is used by classes such as Object
	// and Hash to provide a well-performing mapping between VALUEs.
	template <typename Comparator, typename Allocator>
	class ValueMap : public IGarbage {
	public:
		GC_ROOTS { gc_root_node(_gc, _op, m_Tree.root()); }
		
		typedef snow::RBTree<VALUE, VALUE, Comparator, Allocator> InternalTree;
		typedef VALUE value_type;
		typedef typename InternalTree::Iterator iterator;
		typedef typename InternalTree::ConstIterator const_iterator;
		typedef typename InternalTree::Node Node;
	private:
		void gc_root_node(IGarbageCollector& gc, IGarbageCollector::GCOperation op, Node*& node);
		
		// ValueMap pointers may not be casted to VALUE, so there is no reason to implement gc locking, since a ValueMap can not contain a reference to itself without also containing the object containing the ValueMap, which will have a gc lock.
		inline bool gc_try_lock() { return true; }
		inline void gc_unlock() {}

		InternalTree m_Tree;
	public:
		iterator begin() { return m_Tree.begin(); }
		iterator end() { return m_Tree.end(); }
		const_iterator begin() const { return m_Tree.begin(); }
		const_iterator end() const { return m_Tree.end(); }

		// Returns NULL if key is not found, otherwise the value.
		VALUE find(VALUE key) const;

		void insert(VALUE key, VALUE val) { m_Tree.insert(key, val); }

		// Returns NULL if the key is not found, otherwise the value.
		VALUE erase(VALUE key);

		size_t size() const { return m_Tree.size(); }
	
		VALUE& operator[](VALUE key) { return m_Tree[key]; }
	};

	// ImmediateMap is way faster than ObjectMap, since ObjectMap needs to
	// call actual functions on the objects, while ImmediateMap can work
	// by only looking at the VALUE pointers.
	typedef ValueMap<ImmediateComparator, GarbageAllocatorProxy> ImmediateMap;
	typedef ValueMap<ObjectComparator, GarbageAllocatorProxy> ObjectMap;

	inline int ImmediateComparator::operator()(VALUE _a, VALUE _b) const {
		ASSERT(!is_object(_a) && !is_object(_b));
		uint64_t a = reinterpret_cast<uint64_t>(_a);
		uint64_t b = reinterpret_cast<uint64_t>(_b);
		return a < b ? -1 : (a > b ? 1 : 0);
	}

	inline int ObjectComparator::operator()(VALUE _a, VALUE _b) const {
		bool equals = snow::equals(_a, _b);
		if (equals) return 0;
		// else, compare the object ids
		uint64_t a = snow::get_object_id(_a);
		uint64_t b = snow::get_object_id(_b);
		return a < b ? -1 : (a > b ? 1 : 0);
	}

	inline void* GarbageAllocatorProxy::alloc(size_t sz) {
		return new(kGarbage, kBlob) byte[sz];
	}

	template <typename C, typename A>
	inline VALUE ValueMap<C, A>::find(VALUE key) const {
		const_iterator iter = m_Tree.find(key);
		if (iter != m_Tree.end())
			return iter->value;
		return NULL;
	}

	template <typename C, typename A>
	inline VALUE ValueMap<C, A>::erase(VALUE key) {
		VALUE erased = NULL;
		m_Tree.erase(key, &erased);
		return erased;
	}

	template <typename C, typename A>
	inline void ValueMap<C, A>::gc_root_node(IGarbageCollector& _gc, IGarbageCollector::GCOperation _op, Node*& node) {
		if (!node) return;
		GC_ROOT(node);
		GC_ROOT(node->parent);
		GC_ROOT(node->key);
		GC_ROOT(node->value);
		gc_root_node(_gc, _op, node->left);
		gc_root_node(_gc, _op, node->right);
	}
}

#endif // VALUEMAP_H_QBVHSP4


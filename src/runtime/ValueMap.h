#ifndef VALUEMAP_H_QBVHSP4
#define VALUEMAP_H_QBVHSP4

#include "runtime/Value.h"
#include "runtime/Runtime.h"
#include "gc/IGarbage.h"
#include "base/RBTree.h"

namespace snow {
	class ImmediateComparator {
	public:
		int operator()(const Value& a, const Value& b) const;
	};

	class ObjectComparator {
	public:
		int operator()(const Value& a, const Value& b) const;
	};

	// ValueMap is a simple (and naïve) implementation of a hash map. It
	// cannot be used directly in Snow, but is used by classes such as Object
	// and Hash to provide a well-performing mapping between Values.
	template <typename KeyComparator>
	class ValueMap : public IGarbage {
	public:
		GC_ROOTS { gc_root_node(_gc, _op, m_Tree.root()); }
		
		typedef snow::RBTree<Value, Value, KeyComparator> InternalTree;
		typedef Value value_type;
		typedef typename InternalTree::Iterator iterator;
		typedef typename InternalTree::ConstIterator const_iterator;
		typedef typename InternalTree::Node Node;
	private:
		void gc_root_node(GarbageAllocator& gc, GCOperation op, Node*& node);
		
		// ValueMap pointers may not be casted to Value, so there is no reason to implement gc locking, since a ValueMap can not contain a reference to itself without also containing the object containing the ValueMap, which will have a gc lock.
		inline bool gc_try_lock() { return true; }
		inline void gc_unlock() {}

		InternalTree m_Tree;
	public:
		iterator begin() { return m_Tree.begin(); }
		iterator end() { return m_Tree.end(); }
		const_iterator begin() const { return m_Tree.begin(); }
		const_iterator end() const { return m_Tree.end(); }

		// Returns NULL if key is not found, otherwise the value.
		Value find(const Value& key) const;

		void insert(const Value& key, const Value& val) { m_Tree.insert(key, val); }

		// Returns NULL if the key is not found, otherwise the value.
		Value erase(const Value& key);

		size_t size() const { return m_Tree.size(); }
	
		Value& operator[](const Value& key) { return m_Tree[key]; }
	};

	// ImmediateMap is way faster than ObjectMap, since ObjectMap needs to
	// call actual functions on the objects, while ImmediateMap can work
	// by only looking at the Value pointers.
	typedef ValueMap<ImmediateComparator> ImmediateMap;
	typedef ValueMap<ObjectComparator> ObjectMap;

	inline int ImmediateComparator::operator()(const Value& _a, const Value& _b) const {
		ASSERT(!_a.is_object() && !_b.is_object());
		uintx a = reinterpret_cast<uintx>(_a.value());
		uintx b = reinterpret_cast<uintx>(_b.value());
		return b - a;
	}

	inline int ObjectComparator::operator()(const Value& _a, const Value& _b) const {
		bool equals = snow::equals(_a, _b);
		if (equals) return 0;
		// else, compare the object ids
		uintx a = snow::get_object_id(_a);
		uintx b = snow::get_object_id(_b);
		return b - a;
	}

	template <typename C>
	inline Value ValueMap<C>::find(const Value& key) const {
		const_iterator iter = m_Tree.find(key);
		if (iter != m_Tree.end())
			return iter->value;
		return NULL;
	}

	template <typename C>
	inline Value ValueMap<C>::erase(const Value& key) {
		Value erased;
		m_Tree.erase(key, &erased);
		return erased;
	}

	template <typename C>
	inline void ValueMap<C>::gc_root_node(GarbageAllocator& _gc, GCOperation _op, Node*& node) {
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


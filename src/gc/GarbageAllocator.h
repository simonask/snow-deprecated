#ifndef GARBAGEALLOCATOR_H_3WE55TYZ
#define GARBAGEALLOCATOR_H_3WE55TYZ

#include "MemoryManager.h"
#include "GarbageHeaps.h"
#include "runtime/Handle.h"
#include "gc/IGarbage.h"
#include <unordered_map>
#include <vector>

namespace snow {
	struct GarbageHeader;

	class GarbageAllocator : public IAllocator {
	public:
		typedef GarbageHeader Header;
	private:
		IAllocator::Statistics m_Statistics;
		volatile bool m_IsCollecting;

		NurseryHeap m_NurseryHeap;
		AdultHeap m_AdultHeap;
		int m_AdultHeapBucketsThreshold;
		int m_MinorCollectionsSinceLastMajorCollection;

		std::vector<Ptr<IGarbage>> m_ExternalRoots;
		std::unordered_map<void*, void*> m_MovedPointers;
		
		Header* find_header(void* ptr);
		bool is_blob(void* ptr);
		
		void update_moved(void*& ptr);
		void mark_reachable(void*& ptr);

		void unmark_heap(IGarbageHeap& heap);
		void unmark_all();
		void update_all_moved();
		void mark_all_reachable();

		void inspect_moved_pointers();

		void with_each_root(GCOperation, bool update_stack_frames = false);
		void operation(GCOperation, void*& root);
		void perform_operation(GCOperation, IGarbage* object);
	public:
		GarbageAllocator();
		void* allocate(size_t sz, AllocationType type);
		size_t size_of(void* ptr);
		const IAllocator::Statistics& statistics() const { return m_Statistics; }
		bool contains(void* ptr) const;
		
		void collect();
		
		// all a callback for each type of pointer
		void root_callback(GCOperation, Value&);
		template <typename T>
		void root_callback(GCOperation, Ptr<T>&);
		template <typename T>
		void root_callback(GCOperation, DataPtr<T>&);
		template <typename T>
		void root_callback(GCOperation, T*&);

		void register_root(const Ptr<IGarbage>& ptr);
		void unregister_root(const Ptr<IGarbage>& ptr);

		void destruct(GarbageHeader&, void*);
		void pointer_moved(void* from, void* to, size_t size);
	};
	
	template <typename T>
	inline void GarbageAllocator::root_callback(GCOperation op, Ptr<T>& root) {
		if (root) {
			operation(op, root.gc_root());
			
			// TODO: Get rid of the casting.
			Ptr<IGarbage> iroot = root;
			if (iroot->gc_try_lock()) {
				iroot->_gc_roots(*this, op);
				iroot->gc_unlock();
			}
		}
	}
	
	template <typename T>
	inline void GarbageAllocator::root_callback(GCOperation op, DataPtr<T>& root) {
		operation(op, root.gc_root());
	}
	
	template <typename T>
	inline void GarbageAllocator::root_callback(GCOperation op, T*& root) {
		typedef typename SelectSmartPointerType<T, std::is_pod<T>::value || !(std::is_base_of<IGarbage, T>::value)>::PointerType PointerType;
		PointerType& ptr = reinterpret_cast<PointerType&>(root);
		root_callback(op, ptr);
	}
}

#endif /* end of include guard: GARBAGEALLOCATOR_H_3WE55TYZ */

#ifndef GARBAGEALLOCATOR_H_3WE55TYZ
#define GARBAGEALLOCATOR_H_3WE55TYZ

#include "MemoryManager.h"
#include "Delegate.h"
#include "IGarbage.h"
#include "GarbageHeaps.h"
#include <list>
#include <vector>

namespace snow {
	typedef void(*GarbageFreeFunc)(void* ptr, size_t size);
	
	struct GarbageHeader {
		// sizeof(Header) should be == 16 (128 bits)
		unsigned size           : 32;
		unsigned flags          : 16;
		unsigned generation     : 16;
		GarbageFreeFunc free_func;  // : 64;
	};
	
	enum GarbageFlags {
		GC_NO_FLAGS        = 0,
		GC_FLAG_REACHABLE  = 1,        // Object is referenced, don't delete
		GC_FLAG_BLOB       = 1 << 1,   // Object doesn't have a destructor
		GC_FLAG_DESTRUCTED = 1 << 2,   // Object has been destroyed manually, so don't call destructor again.
	};

	class GarbageAllocator : public IAllocator {
	public:
		typedef GarbageHeader Header;
	private:
		struct MovedPointerInfo {
			void* old_base;
			size_t size;
			void* new_base;

			bool contains(const void* old_ptr) const;
			void* transform(void* old_ptr) const;
		};

		IAllocator::Statistics m_Statistics;
		volatile bool m_IsCollecting;

		NurseryHeap m_NurseryHeap;
		AdultHeap m_AdultHeap;
		int m_AdultHeapBucketsThreshold;
		int m_MinorCollectionsSinceLastMajorCollection;

		std::vector<IGarbage*> m_ExternalRoots;
		std::list<MovedPointerInfo> m_MovedPointers;
		
		Header* find_header(void* ptr);
		
		GCFunc m_MarkReachableDelegate;
		GCFunc m_UpdateMovedDelegate;

		void update_moved(void*& ptr, const char* file, int line);
		void mark_reachable(void*& ptr, const char* file, int line);

		void unmark_heap(IGarbageHeap& heap);
		void unmark_all();
		void update_all_moved();
		void mark_all_reachable();

	public:
		GarbageAllocator();
		void* allocate(size_t sz, AllocationType type);
		size_t size_of(void* ptr);
		const IAllocator::Statistics& statistics() const { return m_Statistics; }
		bool contains(void* ptr) const;
		
		void collect();

		void register_root(IGarbage* ptr);
		void unregister_root(IGarbage* ptr);

		void destruct(GarbageHeader&, void*);
		void pointer_moved(void* from, void* to, size_t size);
	};
}

#endif /* end of include guard: GARBAGEALLOCATOR_H_3WE55TYZ */

#ifndef GARBAGEALLOCATOR_H_3WE55TYZ
#define GARBAGEALLOCATOR_H_3WE55TYZ

#include "MemoryManager.h"
#include "Delegate.h"
#include "IGarbage.h"
#include <list>
#include <vector>

namespace snow {
	class GarbageAllocator : public IAllocator {
	public:
		typedef void(*FreeFunc)(void* ptr, size_t size);
	private:
		class Heap;
		struct Header;
		struct MovedPointerInfo {
			void* old_base;
			size_t size;
			void* new_base;

			bool contains(const void* old_ptr) const;
			void* transform(void* old_ptr) const;
		};


		IAllocator::Statistics m_Statistics;

		// Heaps
		Heap* m_Nursery;
		Heap* m_Mature;
		std::vector<Heap*> m_RealLife;

		std::vector<IGarbage*> m_ExternalRoots;
		std::list<MovedPointerInfo> m_MovedPointers;
		
		Heap& nursery();
		Heap& mature();

		Header* find_header(void* ptr);
		void call_destructors(Heap& heap);
		int64_t for_each_root(GCFunc func);
		
		GCFunc m_MarkDelegate;
		GCFunc m_UpdateMovedDelegate;

		void update_moved(void*& ptr, const char* file, int line);
		void mark(void*& ptr, const char* file, int line);
	public:
		GarbageAllocator();
		void* allocate(size_t sz, AllocationType type);
		size_t size_of(void* ptr);
		const IAllocator::Statistics& statistics() const { return m_Statistics; }
		bool contains(void* ptr) const;
		
		void collect();

		void register_root(IGarbage* ptr);
		void unregister_root(IGarbage* ptr);
	};
}

#endif /* end of include guard: GARBAGEALLOCATOR_H_3WE55TYZ */

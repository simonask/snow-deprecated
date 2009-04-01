#ifndef GARBAGEALLOCATOR_H_3WE55TYZ
#define GARBAGEALLOCATOR_H_3WE55TYZ

#include "MemoryManager.h"
#include <vector>

namespace snow {
	class GarbageAllocator : public IAllocator {
	public:
		typedef void(*FreeFunc)(void* ptr, size_t size);

		
	private:
		class Heap;
		struct Header;
		struct MovedPointerInfo;
		typedef void(*WalkFunc)(void** ptr, void* userdata);

		IAllocator::Statistics m_Statistics;
		Heap* m_Nursery;
		Heap* m_Mature;
		std::vector<Heap*> m_RealLife;
		
		Heap& nursery();
		Heap& mature();

		Header* find_header(void* ptr);
		void call_destructors(Heap& heap);
		int64_t for_each_root(WalkFunc func, void* userdata = NULL);
	public:
		GarbageAllocator();
		void* allocate(size_t sz, AllocationType type);
		size_t size_of(void* ptr);
		const IAllocator::Statistics& statistics() const { return m_Statistics; }
		bool contains(void* ptr) const;
		
		void collect();
		void mark(void* ptr);
	};
}

#endif /* end of include guard: GARBAGEALLOCATOR_H_3WE55TYZ */

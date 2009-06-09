#include "ExecutableAllocator.h"

#include <sys/mman.h>
#include <unistd.h>

#define DEFAULT_HEAP_SIZE (1<<12)

namespace snow {
	class ExecutableAllocator::IncrementalHeap {
	private:
		byte* m_Data;
		size_t m_Size;
		size_t m_Offset;
		bool m_Locked;
		void* m_LastAllocated;
		IncrementalHeap(const IncrementalHeap& other) {}
	public:
		IncrementalHeap(size_t size) : m_Size(size), m_Offset(0), m_Locked(false), m_LastAllocated(NULL) {
			m_Data = (byte*)valloc(size);
		}
		~IncrementalHeap() { memset(m_Data, 0xef, m_Size); ::free(m_Data); }
		
		void* data() const { return m_Data; }
		int size() const { return m_Size; }
		size_t available() const { return locked() ? 0 : m_Size - m_Offset; }
		bool locked() const { return m_Locked; }
		void* last_allocated() const { return m_LastAllocated; }
		
		void lock() { m_Locked = true; }
		void unlock() { m_Locked = false; }
		
		void* allocate(size_t len) {
			if (available() >= len) {
				byte* ptr;

				#pragma omp critical
				{
					ptr = &m_Data[m_Offset];
					m_Offset += len;
					m_LastAllocated = (void*)ptr;
				}
				
				#ifdef DEBUG
				// Poison
				memset(ptr, 0xcd, len);
				#endif
				
				return ptr;
			} else
				return NULL;
		}
		
		bool deallocate(void* ptr) {
			/*if (ptr == m_LastAllocated) {
				ptrdiff_t len = ((int64_t)ptr - (int64_t)m_Data);
				m_Offset -= len;
				
				return true;
			}*/
			return false;
		}
		
		bool contains(void* ptr) const {
			return ptr >= m_Data && ptr < &m_Data[m_Offset];
		}
		
		void make_executable() {
			mprotect(m_Data, m_Size, PROT_EXEC);
			lock();
		}
		
		void make_writable() {
			mprotect(m_Data, m_Size, PROT_WRITE);
			unlock();
		}
	};
	
	void* ExecutableAllocator::allocate(size_t len, AllocationType) {
		IncrementalHeap* heap = NULL;
		for each (iter, m_Heaps) {
			if ((*iter)->available() >= len) {
				heap = *iter;
				break;
			}
		}
		
		if (!heap) {
			heap = new IncrementalHeap(len > DEFAULT_HEAP_SIZE ? len : DEFAULT_HEAP_SIZE);
			m_Heaps.push_back(heap);
		}
		
		m_Stats.allocated_size += len;
		m_Stats.allocated_objects++;
		
		return heap->allocate(len);
	}
	
	ExecutableAllocator::IncrementalHeap* ExecutableAllocator::heap_containing(void* ptr) {
		for each_reverse (iter, m_Heaps) {
			IncrementalHeap* heap = *iter;
			if (heap->contains(ptr)) {
				return heap;
			}
		}
		return NULL;
	}
	
	void ExecutableAllocator::make_executable(void* ptr) {
		IncrementalHeap* heap = heap_containing(ptr);
		if (heap)
			heap->make_executable();
	}
	
	void ExecutableAllocator::make_writable(void* ptr) {
		IncrementalHeap* heap = heap_containing(ptr);
		if (heap)
			heap->make_writable();
	}
}

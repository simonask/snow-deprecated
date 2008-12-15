#include "IncrementalAlloc.h"

#include <vector>
using namespace std;

#include <sys/mman.h>
#include <unistd.h>

#define DEFAULT_HEAP_SIZE (1<<15)

namespace snot {
	class IncrementalHeap {
	private:
		unsigned char* m_Data;
		size_t m_Size;
		size_t m_Offset;
		bool m_Locked;
		void* m_LastAllocated;
		IncrementalHeap(const IncrementalHeap& other) {}
	public:
		IncrementalHeap(size_t size) : m_Size(size), m_Offset(0), m_Locked(false), m_LastAllocated(NULL) {
			m_Data = (unsigned char*)valloc(size);
		}
		~IncrementalHeap() { free(m_Data); }
		
		void* data() const { return m_Data; }
		int size() const { return m_Size; }
		int available() const { return locked() ? 0 : m_Size - m_Offset; }
		bool locked() const { return m_Locked; }
		void* last_allocated() const { return m_LastAllocated; }
		
		void lock() { m_Locked = true; }
		void unlock() { m_Locked = false; }
		
		void* allocate(size_t len) {
			if (available() >= len) {
				unsigned char* ptr = &m_Data[m_Offset];
				m_Offset += len;
				m_LastAllocated = (void*)ptr;
				return m_LastAllocated;
			} else
				return NULL;
		}
		
		bool deallocate(void* ptr) {
			if (ptr == m_LastAllocated) {
				m_Offset -= ((int64_t)ptr - (int64_t)m_Data);
				return true;
			}
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
	
	static vector<IncrementalHeap*> g_heaps;
	
	void* incremental_alloc(size_t len) {
		IncrementalHeap* heap = NULL;
		for (vector<IncrementalHeap*>::reverse_iterator iter = g_heaps.rbegin(); iter != g_heaps.rend(); ++iter) {
			if ((*iter)->available() >= len) {
				heap = *iter;
				break;
			}
		}
		
		if (!heap) {
			heap = new IncrementalHeap(len > DEFAULT_HEAP_SIZE ? len : DEFAULT_HEAP_SIZE);
			g_heaps.push_back(heap);
		}
		
		return heap->allocate(len);
	}
	
	void incremental_free(void* ptr) {
		for (vector<IncrementalHeap*>::reverse_iterator iter = g_heaps.rbegin(); iter < g_heaps.rend(); ++iter) {
			if ((*iter)->deallocate(ptr))
				break;
		}
	}
	
	static IncrementalHeap* heap_containing_pointer(void* ptr) {
		for (vector<IncrementalHeap*>::reverse_iterator iter = g_heaps.rbegin(); iter < g_heaps.rend(); ++iter) {
			IncrementalHeap* heap = *iter;
			if (heap->contains(ptr)) {
				return heap;
			}
		}
		return NULL;
	}
	
	void incremental_make_executable(void* ptr) {
		IncrementalHeap* heap = heap_containing_pointer(ptr);
		if (heap)
			heap->make_executable();
	}
	
	void incremental_make_writable(void* ptr) {
		IncrementalHeap* heap = heap_containing_pointer(ptr);
		if (heap)
			heap->make_writable();
	}
}
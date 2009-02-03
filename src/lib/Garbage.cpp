#include "Garbage.h"
#include "Handle.h"
#include "Object.h"
#include <pthread.h>
#include <unordered_map>

namespace snow {
	#define ALIGNMENT 0x10
	
	class MemoryHeap {
	private:
		size_t m_Size;
		size_t m_Offset;
		byte* m_Data;
	public:
		typedef std::unordered_map<void*, void*> PointerMap;
		
		explicit MemoryHeap(size_t sz) : m_Size(sz), m_Offset(0) {
			m_Data = new byte[sz];
		}
		~MemoryHeap() { delete[] m_Data; }
		
		inline size_t available() const { return m_Size - m_Offset; }
		
		void* alloc(size_t sz) {
			size_t required_size = sz + (ALIGNMENT - sz % ALIGNMENT);
			if (available() < required_size)
				return NULL;
			Garbage* object = (Garbage*)&m_Data[m_Offset];
			m_Offset += required_size;
			object->m_Size = required_size;
			object->m_Mark = 1;
			return object;
		}
		
		byte* data() { return m_Data; }
		inline bool contains(Garbage* ptr) {
			return (byte*)ptr >= &m_Data[0] && (byte*)ptr < &m_Data[m_Offset];
		}
		
		void compact(PointerMap& moved_pointers) {
		}
		
		class iterator {
			friend class MemoryHeap;
		private:
			MemoryHeap& m_Heap;
			size_t m_Offset;
			iterator(MemoryHeap& heap, size_t offset) : m_Heap(heap), m_Offset(offset) {}
			
			void next() {
				m_Offset += object()->m_Size;
			}
		public:
			iterator(const iterator& other) : m_Heap(other.m_Heap), m_Offset(other.m_Offset) {}
			Garbage* object() const { return (Garbage*)&m_Heap.data()[m_Offset];}
			Garbage* operator*() const { return object(); }
			iterator& operator++() {
				next();
				return *this;
			}
			iterator operator++(int) {
				iterator copy(*this);
				next();
				return copy;
			}
			bool operator==(const iterator& other) const { return &m_Heap == &other.m_Heap && m_Offset == other.m_Offset; }
			bool operator!=(const iterator& other) const { return !(*this == other); }
		};
		
		iterator begin() { return iterator(*this, 0); }
		iterator end() { return iterator(*this, m_Offset); }
		typedef Garbage* value_type;
	};
	
	static pthread_key_t young_key;
	static pthread_key_t old_list_key;
	
	static class StaticInitializer_MemoryHeaps {
	public:
		StaticInitializer_MemoryHeaps() {
			pthread_key_create(&young_key, NULL);
			pthread_key_create(&old_list_key, NULL);
		}
	} _init;
	
	static MemoryHeap* young_heap() {
		MemoryHeap* heap = (MemoryHeap*)pthread_getspecific(young_key);
		if (!heap) {
			heap = new MemoryHeap(1 << 20);
			pthread_setspecific(young_key, (void*)heap);
		}
		return heap;
	}
	
	void* Garbage::operator new(size_t sz) throw() {
		return young_heap()->alloc(sz);
	}
	
	void Garbage::collect() {
		MemoryHeap* heap = young_heap();
		
		// First, reset marks
		for each (iter, *heap) {
			Garbage* object = *iter;
			object->unmark();
		}
		
		// Then, go through the stack, marking objects
		Handle* handle = Handle::last();
		while (handle) {
			if (handle->is_object()) {
				Garbage* object = handle->cast<Garbage>();
				if (heap->contains(object))
					object->mark();
			}
			handle = handle->previous();
		}
		
		// Then, compact heap
		std::unordered_map<void*, void*> moved_pointers;
		
		int count = 0;
		for each (iter, *heap) {
			Garbage* object = *iter;
			if (!object->m_Mark) {
				count++;
				delete object;
			}
		}
		debug("there were %d unmarked objects", count);
		
		// And finally, update stack pointers
	}
}
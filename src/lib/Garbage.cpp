#include "Garbage.h"
#include "Handle.h"
#include "Object.h"
#include "StackFrame.h"
#include "Runtime.h"
#include <pthread.h>
#include <unordered_map>
#include <string.h>

namespace snow {
	#define ALIGNMENT 0x10
	
	Garbage::Stats Garbage::stats = Garbage::Stats();
	
	class MemoryHeap {
	private:
		size_t m_Size;
		size_t m_Offset;
		byte* m_Data;
	public:
		typedef std::unordered_map<void*, void*> PointerMap;
		
		explicit MemoryHeap(size_t sz) : m_Size(sz), m_Offset(0) {
			m_Data = new byte[sz];
			
			#ifdef DEBUG
			// poison
			memset(m_Data, 0xcd, sz);
			#endif
		}
		~MemoryHeap() { delete[] m_Data; }
		
		inline size_t available() const { return m_Size - m_Offset; }
		
		void* alloc(size_t sz, bool blob) {
			size_t required_size = sizeof(Garbage::Header) + sz;
			required_size += (ALIGNMENT - required_size % ALIGNMENT);
			if (available() < required_size)
				return NULL;
				
			Garbage::Header* header = (Garbage::Header*)&m_Data[m_Offset];
			Garbage* object = (Garbage*)&m_Data[m_Offset + sizeof(Garbage::Header)];
			m_Offset += required_size;
			
			header->size = required_size;
			header->flags = Garbage::NO_FLAGS;
			if (blob)
				header->flags |= Garbage::BLOB;
			header->generation = 0;
			
			return object;
		}
		
		byte* data() { return m_Data; }
		
		inline bool contains(const void* ptr) {
			return (byte*)ptr >= &m_Data[0] && (byte*)ptr < &m_Data[m_Offset];
		}
		
		void compact(PointerMap& moved_pointers) {
			// TODO: It.
		}
		
		class iterator {
			friend class MemoryHeap;
		private:
			MemoryHeap& m_Heap;
			size_t m_Offset;
			iterator(MemoryHeap& heap, size_t offset) : m_Heap(heap), m_Offset(offset) {}
			
			void next() {
				m_Offset += header()->size;
			}
		public:
			iterator(const iterator& other) : m_Heap(other.m_Heap), m_Offset(other.m_Offset) {}
			Garbage::Header* header() const { return (Garbage::Header*)&m_Heap.data()[m_Offset]; }
			void* pointer() const { return (void*)&m_Heap.data()[m_Offset + sizeof(Garbage::Header)];}
			void* operator*() const { return pointer(); }
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
		typedef void* value_type;
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
			heap = new MemoryHeap(1 << 22);
			pthread_setspecific(young_key, (void*)heap);
		}
		return heap;
	}
	
	Garbage::Header* Garbage::header(const void* ptr) {
		if (young_heap()->contains(ptr)) {
			ptrdiff_t offset = (ptrdiff_t)ptr - (ptrdiff_t)young_heap()->data();
			Header* h = (Header*)&young_heap()->data()[offset - sizeof(Header)];
			return h;
		}
		return NULL;
	}
	
	void* Garbage::alloc(size_t sz, bool blob) {
		void* ptr = young_heap()->alloc(sz, blob);
		stats.allocated_objects++;
		stats.allocated_size += Garbage::header(ptr)->size;;
		if (!ptr) TRAP();
		return ptr;
	}
	
	void Garbage::_mark(void* ptr) {
		Header* h = header(ptr);
		if (h) {
			h->flags |= MARKED;
			++h->generation;
		}
	}
	
	void Garbage::mark(void* ptr) {
		Header* h = header(ptr);
		if (h) {
			if (h->flags & BLOB)
				_mark(ptr);
			else
				static_cast<Garbage*>(ptr)->mark(ptr);
		}
	}
	
	bool Garbage::is_marked(void* ptr) {
		Header* h = header(ptr);
		if (h) {
			return h->flags & MARKED;
		}
		return false;
	}
	
	bool Garbage::is_blob(void* ptr) {
		Header* h = header(ptr);
		if (h) {
			return h->flags & BLOB;
		}
		return false;
	}
	
	inline void Garbage::mark_frame(StackFrame* frame) {
		// XXX: TODO
	}
	
	void Garbage::collect() {
		MemoryHeap* heap = young_heap();
		
		// First, reset marks
		for each (iter, *heap) {
			Header* h = iter.iterator().header();
			if (h) {
				h->flags |= ~MARKED;
			} else {
				error("Memory corruption!");
				TRAP();
			}
		}
		
		// Then, go through the stack, marking objects
		// First, the C++ stack...
		ValueHandle* handle = ValueHandle::last();
		while (handle) {
			Garbage::mark(handle);
			handle = handle->previous();
			if (handle == handle->previous()) {
				error("Circular Handle references!");
				TRAP();
			}
		}
		// Then, the Snow stack...
		StackFrame* frame = get_current_stack_frame();
		while (frame) {
			mark_frame(frame);
			frame = frame->previous;
		}
		// Then, the static scopes of Snow...
		frame = get_current_stack_frame();
		// Marking done.
		
		// Then, compact heap
		std::unordered_map<void*, void*> moved_pointers;
		
		int count = 0;
		for each (iter, *heap) {
			if (!Garbage::is_marked(*iter)) {
				if (!Garbage::is_blob(*iter)) {
					Garbage* object = static_cast<Garbage*>(*iter);
					delete object;
				}
				stats.collected_objects++;
				stats.collected_size += header(*iter)->size;
			}
		}
		debug("there were %d unmarked objects", count);
		
		// And finally, update stack pointers
	}
}
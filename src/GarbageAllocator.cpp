#include "GarbageAllocator.h"
#include "IGarbage.h"
#include "Handle.h"
#include "lib/Runtime.h"

#include <set>
#include <list>

namespace snow {
	// Constants that can be tweaked
	static const size_t NURSERY_SIZE = (1<<20);     // 1 Mb
	static const size_t MATURE_SIZE = (1<<25);      // 32 Mb
	static const size_t REALLIFE_HEAP_SIZE = (1<<23);    // 8 Mb per RealLife heap
	static const size_t REALLIFE_HEAP_NUMBER_THRESHOLD = 10; // begin looking for garbage in reallife heaps when n heaps have been filled.

	static const size_t NURSERY_TO_MATURE_GENERATION = 10;
	static const size_t NURSERY_TO_REALLIFE_GENERATION = 20;

	// Must be 0x10 (16), otherwise we can't tell pointers from immediates.
	static const size_t ALIGNMENT = 0x10;
	
	/// SUPPORTING STRUCTURES ///  

	struct GarbageAllocator::Header {
		// sizeof(Header) should be == 16 (128 bits)
		unsigned size           : 32;
		unsigned flags          : 16;
		unsigned generation     : 16;
		GarbageAllocator::FreeFunc free_func;  // : 64;
	};

	enum Flags {
		NO_FLAGS        = 0,
		FLAG_REACHABLE  = 1,        // Object is referenced, don't delete
		FLAG_BLOB       = 1 << 1,   // Object doesn't have a destructor
		FLAG_DESTRUCTED = 1 << 2,   // Object has been destroyed manually, so don't call destructor again.
	};

	struct GarbageAllocator::MovedPointerInfo {
		void* old_base;
		size_t size;
		void* new_base;

		bool contains(const void* old_ptr) const {
			const byte* _old_ptr = reinterpret_cast<const byte*>(old_ptr);
			const byte* _old_base = reinterpret_cast<const byte*>(old_base);
			return (_old_ptr >= _old_base && _old_ptr < &_old_base[size]);
		}

		void* transform(void* old_ptr) {
			if (contains(old_ptr)) {
				const byte* _old_ptr = reinterpret_cast<const byte*>(old_ptr);
				const byte* _old_base = reinterpret_cast<const byte*>(old_base);
				byte* _new_base = reinterpret_cast<byte*>(new_base);
				ptrdiff_t offset = _old_ptr - _old_base;
				return &_new_base[offset];
			}
			return old_ptr;
		}
	};

	class GarbageAllocator::Heap {
	public:
		class Enumerator;
		friend class Enumerator;
	private:
		byte* m_Data;
		const size_t m_Size;
		size_t m_Offset;
	public:	
		class Enumerator {
			friend class GarbageAllocator::Heap;
			Heap& m_Heap;
			byte* m_Current;
			Enumerator(Heap& heap) : m_Heap(heap), m_Current(heap.m_Data) {}
		public:
			Enumerator(const Enumerator& other) : m_Heap(other.m_Heap), m_Current(other.m_Current) {} 
			bool next() {
				if (m_Current < &m_Heap.m_Data[m_Heap.m_Offset]) {
					m_Current += sizeof(Header) + header().size;
					return true;
				}
				return false;
			}

			Header& header() const {
				return *reinterpret_cast<Header*>(m_Current);
			}

			void* data() const {
				return reinterpret_cast<void*>(&m_Current[sizeof(Header)]);
			}
		};

		Enumerator get_enumerator() { return Enumerator(*this); }

		Heap(size_t sz) : m_Data(NULL), m_Size(sz), m_Offset(0) {
			m_Data = new(kMalloc) byte[sz];
			ASSERT(m_Data && "Could not allocate heap. Probably out of memory, or heap size too large!");
		}
		
		~Heap() {
			delete[] m_Data;
		}

		size_t size() const { return m_Size; }
		byte* data() const { return m_Data; }
		size_t offset() const { return m_Offset; }

		size_t available() const {
			return m_Size - m_Offset;
		}

		bool contains(const void* ptr) const {
			return m_Data <= (byte*)ptr && &m_Data[m_Offset] > (byte*)ptr;
		}
		
		void* allocate(size_t sz, GarbageAllocator::Header** pheader) {
			size_t required_size = sizeof(Header) + sz;
			required_size += (ALIGNMENT - required_size % ALIGNMENT);
			
			if (available() < required_size)
				return NULL;
				
			byte* data = &m_Data[m_Offset];
			*pheader = reinterpret_cast<Header*>(&data[0]);
			void* object = reinterpret_cast<void*>(&data[sizeof(Header)]);

			m_Offset += required_size;
			
			(*pheader)->size = required_size - sizeof(Header);
			(*pheader)->flags = NO_FLAGS;
			(*pheader)->generation = 0;
			(*pheader)->free_func = NULL;
			
			return object;
		}

		void unmark_objects() {
			Enumerator iter = get_enumerator();
			while (iter.next()) {
				iter.header().flags &= ~FLAG_REACHABLE;
			}
		}

	};

	GarbageAllocator::Header* GarbageAllocator::find_header(void* ptr) {
		if (contains(ptr)) {
			byte* data = reinterpret_cast<byte*>(ptr);
			Header* header = reinterpret_cast<Header*>(data - (int)sizeof(Header)); 
		}
		return NULL;
	}
	
	GarbageAllocator::GarbageAllocator() : m_Nursery(NULL), m_Mature(NULL) {}
	
	GarbageAllocator::Heap& GarbageAllocator::nursery() {
		if (!m_Nursery)
			m_Nursery = new(kMalloc) Heap(NURSERY_SIZE);
		ASSERT(m_Nursery);
		return *m_Nursery;
	}
	
	GarbageAllocator::Heap& GarbageAllocator::mature() {
		if (!m_Mature)
			m_Mature = new(kMalloc) Heap(MATURE_SIZE);
		ASSERT(m_Mature);
		return *m_Mature;
	}
	
	
	
	int64_t GarbageAllocator::for_each_root(GarbageAllocator::WalkFunc func, void* userdata) {
		int64_t num_roots = 0;
		
		// First, handles and permanents
		for each (iter, ValueHandle::list()) {
			ValueHandle* handle = *iter;
			func(handle->value_ptr(), userdata);
			++num_roots;
		}
		
		// Then, the Snow stack
		StackFrame* frame = get_current_stack_frame();
		while (frame) {
			func(reinterpret_cast<void**>(&frame->self), userdata);
			func(reinterpret_cast<void**>(&frame->it), userdata);
			num_roots += 2;
			
			func(reinterpret_cast<void**>(&frame->scope), userdata);
			
			for (size_t i = 0; i < frame->scope->arguments()->length(); ++i) {
				func(&(frame->scope->arguments()->data()[i]), userdata);
			}
			num_roots += frame->scope->arguments()->length();
			
			for (size_t i = 0; i < frame->scope->locals()->length(); ++i) {
				func(&(frame->scope->locals()->data()[i]), userdata);
			}
			num_roots += frame->scope->locals()->length();
			
			frame = frame->previous;
		}
		
		return num_roots;
	}

	void* GarbageAllocator::allocate(size_t sz, snow::AllocationType type) {
		Header* header;
		void* ptr = nursery().allocate(sz, &header);
		
		header->flags = NO_FLAGS;
		header->generation = 0;
		header->free_func = NULL;
		
		switch (type) {
			case kArray:
				error("Garbage collector does not support array allocations. Use kBlob allocation if you need a block consisting of a POD type.");
				TRAP();
			case kBlob:
				header->flags |= FLAG_BLOB;
				break;
			case kObject:
				break;
		}
		
		if (!ptr) {
			// TODO: Minor collection
		}
		m_Statistics.allocated_size += sz;
		m_Statistics.allocated_objects++;
		return ptr;
	}
	
	size_t GarbageAllocator::size_of(void* ptr) {
		return 0;
	}
	
	bool GarbageAllocator::contains(void* ptr) const {
		if (m_Nursery && m_Nursery->contains(ptr)) {
			return true;
		}
		if (m_Mature && m_Mature->contains(ptr)) {
			return true;
		}
		
		for each (iter, m_RealLife) {
			if ((*iter)->contains(ptr))
				return true;
		}
		
		return false;
	}
	
	void GarbageAllocator::call_destructors(GarbageAllocator::Heap& heap) {
		Heap::Enumerator e = heap.get_enumerator();
		while (e.next()) {
			uint32_t flags = e.header().flags;

			if (!(flags & FLAG_REACHABLE) && !(flags & FLAG_BLOB) && !(flags & FLAG_DESTRUCTED))
			{
				if (!(flags & FLAG_BLOB)) {
					IGarbage* object = reinterpret_cast<IGarbage*>(e.data());
					object->~IGarbage();
					e.header().flags |= FLAG_DESTRUCTED;
				}
				m_Statistics.freed_objects++;
				m_Statistics.freed_size += e.header().size;
			}
		}
	}
	
	void GarbageAllocator::mark(void* ptr) {
		if (contains(ptr)) {
			byte* object = static_cast<byte*>(ptr);
			Header* header = reinterpret_cast<Header*>(object - (ptrdiff_t)sizeof(Header));
			header->flags |= FLAG_REACHABLE;
			if (!(header->flags & FLAG_BLOB)) {
				IGarbage* gc_object = reinterpret_cast<IGarbage*>(object);

				gc_object->gc_func(create_delegate(this, &GarbageAllocator::mark));
			}
		}
	}
	
	void GarbageAllocator::collect() {
		nursery().unmark_objects();
		
		for each (iter, ValueHandle::list()) {
			ValueHandle* handle = *iter;
			mark(handle->value());
		}
		
		StackFrame* frame = get_current_stack_frame();
		while (frame) {
			mark(frame->scope);
			frame = frame->previous;
		}
		
		call_destructors(nursery());
	}
}

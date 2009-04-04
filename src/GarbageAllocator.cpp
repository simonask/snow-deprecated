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


	bool GarbageAllocator::MovedPointerInfo::contains(const void* old_ptr) const {
		const byte* _old_ptr = reinterpret_cast<const byte*>(old_ptr);
		const byte* _old_base = reinterpret_cast<const byte*>(old_base);
		return (_old_ptr >= _old_base && _old_ptr < &_old_base[size]);
	}

	void* GarbageAllocator::MovedPointerInfo::transform(void* old_ptr) const {
		if (contains(old_ptr)) {
			const byte* _old_ptr = reinterpret_cast<const byte*>(old_ptr);
			const byte* _old_base = reinterpret_cast<const byte*>(old_base);
			byte* _new_base = reinterpret_cast<byte*>(new_base);
			ptrdiff_t offset = _old_ptr - _old_base;
			return &_new_base[offset];
		}
		return old_ptr;
	}

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
			Enumerator(Heap& heap) : m_Heap(heap), m_Current(NULL) {}
		public:
			Enumerator(const Enumerator& other) : m_Heap(other.m_Heap), m_Current(other.m_Current) {} 
			bool next() {
				if (!m_Current) {
					if (m_Heap.m_Offset > 0)
					{
						m_Current = m_Heap.m_Data;
						return true;
					}
					return false;
				}
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

			void* offset() const {
				return m_Current;
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
			return &m_Data[sizeof(Header)] <= (byte*)ptr && &m_Data[m_Offset] > (byte*)ptr;
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

		std::list<MovedPointerInfo> compact(size_t* deleted_blocks = NULL, size_t* deleted_bytes = NULL) {
			byte* new_data = new(kMalloc) byte[m_Size];
			size_t new_offset = 0;
			std::list<MovedPointerInfo> moved_pointers;

			size_t total_objects = 0;
			size_t moved_objects = 0;

			Enumerator e = get_enumerator();
			while (e.next()) {
				++total_objects;
				if (e.header().flags & FLAG_REACHABLE) {
					e.header().generation++;

					size_t to_copy = sizeof(Header) + e.header().size;
					memcpy(&new_data[new_offset], e.offset(), to_copy);

					MovedPointerInfo mpi;
					mpi.old_base = e.data();
					mpi.size = e.header().size;
					mpi.new_base = &new_data[new_offset+sizeof(Header)];
					moved_pointers.push_back(mpi);

					new_offset += to_copy;

					++moved_objects;
				} else {
					if (!(e.header().flags & FLAG_BLOB) && !(e.header().flags & FLAG_DESTRUCTED)) {
						IGarbage* object = reinterpret_cast<IGarbage*>(e.data());
						object->~IGarbage();
					}
				}
			}
			
			debug("Compacting heap 0x%llx -- %u of %u objects survived.", this, moved_objects, total_objects);
			if (deleted_blocks)
				*deleted_blocks = total_objects - moved_objects;
			if (deleted_bytes)
				*deleted_bytes = m_Offset - new_offset;

			delete[] m_Data;
			m_Data = new_data;
			m_Offset = new_offset;

			return moved_pointers;
		}
	};

	GarbageAllocator::Header* GarbageAllocator::find_header(void* ptr) {
		if (contains(ptr)) {
			byte* data = reinterpret_cast<byte*>(ptr);
			Header* header = reinterpret_cast<Header*>(data - (int)sizeof(Header)); 
			return header;
		}
		return NULL;
	}
	
	GarbageAllocator::GarbageAllocator() :
		m_Nursery(NULL),
		m_Mature(NULL),
		m_MarkDelegate(new MemberDelegate<GarbageAllocator, void, void*&, const char*, int>(this, &GarbageAllocator::mark)),
		m_UpdateMovedDelegate(new MemberDelegate<GarbageAllocator, void, void*&, const char*, int>(this, &GarbageAllocator::update_moved))
	{
	}
	
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
	
	int64_t GarbageAllocator::for_each_root(GCFunc func) {
		int64_t num_roots = 0;
		
		// First, handles and permanents
		for each (iter, ValueHandle::list()) {
			ValueHandle* handle = *iter;
			func(reinterpret_cast<void*&>(handle), __FILE__, __LINE__);
			func(handle->value(), __FILE__, __LINE__);
			++num_roots;
		}
		
		// Then, the Snow stack
		StackFrame* frame = get_current_stack_frame();
		while (frame) {
			func(reinterpret_cast<void*&>(frame->scope), __FILE__, __LINE__);
			update_stack_frame(frame, frame->scope);
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

	void GarbageAllocator::mark(void*& ptr, const char* file, int line) {
		Header* header = find_header(ptr);
		if (header) {
			if (!(header->flags & FLAG_REACHABLE)) {
				header->flags |= FLAG_REACHABLE;
				if (!(header->flags & FLAG_BLOB)) {
					IGarbage* gc_object = reinterpret_cast<IGarbage*>(ptr);

					gc_object->_gc_roots(m_MarkDelegate);
				}
			}
		}
	}
	
	void GarbageAllocator::update_moved(void*& ptr, const char* file, int line) {
		for each (iter, m_MovedPointers) {
			if (iter->contains(ptr)) {
				void* new_ptr = iter->transform(ptr);
				ptr = new_ptr;
			}
		}

		Header* header = find_header(ptr);
		if (header && !(header->flags & FLAG_BLOB)) {
			IGarbage* gc_object = reinterpret_cast<IGarbage*>(ptr);
			gc_object->_gc_roots(m_UpdateMovedDelegate);
		}
	}
	
	void GarbageAllocator::collect() {
		nursery().unmark_objects();
		
		// MARK REACHABLE OBJECTS
		// C++ stack
		for each (iter, ValueHandle::list()) {
			m_MarkDelegate((*iter)->value(), __FILE__, __LINE__);
		}
		// Snow stack
		StackFrame* frame = get_current_stack_frame();
		while (frame) {
			m_MarkDelegate(reinterpret_cast<void*&>(frame->scope), __FILE__, __LINE__);
			frame = frame->previous;
		}
		// External roots
		for each (iter, m_ExternalRoots) {
			(*iter)->_gc_roots(m_MarkDelegate);
		}

		size_t deleted_blocks, deleted_bytes;
		m_MovedPointers = nursery().compact(&deleted_blocks, &deleted_bytes);
		m_Statistics.freed_objects += deleted_blocks;
		m_Statistics.freed_size += deleted_bytes;
		
		// UPDATE MOVED POINTERS
		// C++ stack
		for (auto iter = ValueHandle::list().begin(); iter != ValueHandle::list().end(); ++iter) {
			// the handle itself
			for each (mpi_iter, m_MovedPointers) {
				*iter = static_cast<ValueHandle*>(mpi_iter->transform(*iter));
			}
			// the object
			m_UpdateMovedDelegate((*iter)->value(), __FILE__, __LINE__);
		}
		// Snow stack
		frame = get_current_stack_frame();
		while (frame) {
			void* before = frame->scope;
			m_UpdateMovedDelegate(reinterpret_cast<void*&>(frame->scope), __FILE__, __LINE__);
			update_stack_frame(frame, frame->scope);
			frame = frame->previous;
		}
		// External roots
		for each (iter, m_ExternalRoots) {
			(*iter)->_gc_roots(m_UpdateMovedDelegate);
		}

		m_MovedPointers.clear();
	}

	void GarbageAllocator::register_root(IGarbage* ptr) {
		ASSERT(!contains(ptr));
		m_ExternalRoots.push_back(ptr);
	}

	void GarbageAllocator::unregister_root(IGarbage* ptr) {
		for (auto iter = m_ExternalRoots.begin(); iter != m_ExternalRoots.end(); ++iter) {
			if (*iter == ptr)
				iter = m_ExternalRoots.erase(iter);
		}
	}
}

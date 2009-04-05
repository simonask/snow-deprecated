#include "GarbageAllocator.h"
#include "IGarbage.h"
#include "Handle.h"
#include "lib/Runtime.h"

#include <set>
#include <list>

namespace snow {
	// Constants that can be tweaked
	static const size_t NURSERY_SIZE = (1<<20);     // 1 Mb

	// Must be 0x10 (16), otherwise we can't tell pointers from immediates.
	static const size_t ALIGNMENT = 0x10;
	
	inline bool GarbageAllocator::MovedPointerInfo::contains(const void* old_ptr) const {
		const byte* _old_ptr = reinterpret_cast<const byte*>(old_ptr);
		const byte* _old_base = reinterpret_cast<const byte*>(old_base);
		return (_old_ptr >= _old_base && _old_ptr < &_old_base[size]);
	}

	inline void* GarbageAllocator::MovedPointerInfo::transform(void* old_ptr) const {
		if (contains(old_ptr)) {
			const byte* _old_ptr = reinterpret_cast<const byte*>(old_ptr);
			const byte* _old_base = reinterpret_cast<const byte*>(old_base);
			byte* _new_base = reinterpret_cast<byte*>(new_base);
			ptrdiff_t offset = _old_ptr - _old_base;
			return &_new_base[offset];
		}
		return old_ptr;
	}
	
	GarbageAllocator::GarbageAllocator() :
		m_IsCollecting(false),
		m_NurseryHeap(*this, NURSERY_SIZE),
		m_AdultHeap(*this),
		m_AdultHeapBucketsThreshold(0),
		m_MinorCollectionsSinceLastMajorCollection(0),
		m_MarkReachableDelegate(new MemberDelegate<GarbageAllocator, void, void*&, const char*, int>(this, &GarbageAllocator::mark_reachable)),
		m_UpdateMovedDelegate(new MemberDelegate<GarbageAllocator, void, void*&, const char*, int>(this, &GarbageAllocator::update_moved))
	{
		m_NurseryHeap.set_next_heap(&m_AdultHeap);
	}

	GarbageAllocator::Header* GarbageAllocator::find_header(void* ptr) {
		if (contains(ptr)) {
			byte* data = reinterpret_cast<byte*>(ptr);
			Header* header = reinterpret_cast<Header*>(data - (int)sizeof(Header)); 
			return header;
		}
		return NULL;
	}
	
	
	void* GarbageAllocator::allocate(size_t sz, snow::AllocationType type) {
		ASSERT(!m_IsCollecting && "Cannot perform allocations while collecting garbage!");

		Header* header;
		void* ptr = m_NurseryHeap.allocate(sz, header);

		ASSERT(ptr && "Out of memory?");
		
		header->flags = GC_NO_FLAGS;
		header->generation = 0;
		header->free_func = NULL;
		
		ASSERT((type != kArray) && "Garbage collector does not support array allocations. Use kBlob allocation if you need a block consisting of a POD type.");
		if (type == kBlob)
			header->flags |= GC_FLAG_BLOB;
		
		m_Statistics.allocated_size += sz;
		m_Statistics.allocated_objects++;
		return ptr;
	}
	
	size_t GarbageAllocator::size_of(void* ptr) {
		Header* header = find_header(ptr);
		if (header)
			return header->size;
		return 0;
	}
	
	bool GarbageAllocator::contains(void* ptr) const {
		return m_NurseryHeap.contains(ptr) || m_AdultHeap.contains(ptr);
	}
	
	void GarbageAllocator::mark_reachable(void*& ptr, const char* file, int line) {
		Header* header = find_header(ptr);
		if (header) {
			if (!(header->flags & GC_FLAG_REACHABLE)) {
				header->flags |= GC_FLAG_REACHABLE;
				if (!(header->flags & GC_FLAG_BLOB)) {
					IGarbage* gc_object = reinterpret_cast<IGarbage*>(ptr);

					gc_object->_gc_roots(m_MarkReachableDelegate);
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
		if (header && !(header->flags & GC_FLAG_BLOB)) {
			IGarbage* gc_object = reinterpret_cast<IGarbage*>(ptr);
			gc_object->_gc_roots(m_UpdateMovedDelegate);
		}
	}

	void GarbageAllocator::unmark_heap(IGarbageHeap& heap) {
		IGarbageHeap::Enumerator e(heap);
		while (e.next()) {
			e.header().flags &= ~GC_FLAG_REACHABLE;
		}
	}

	void GarbageAllocator::unmark_all() {
		unmark_heap(m_NurseryHeap);
		unmark_heap(m_AdultHeap);
	}

	void GarbageAllocator::mark_all_reachable() {
		// C++ stack
		for each (iter, ValueHandle::list()) {
			m_MarkReachableDelegate((*iter)->value(), __FILE__, __LINE__);
		}
		// Snow stack
		StackFrame* frame = get_current_stack_frame();
		while (frame) {
			m_MarkReachableDelegate(reinterpret_cast<void*&>(frame->scope), __FILE__, __LINE__);
			frame = frame->previous;
		}
		// External roots
		for each (iter, m_ExternalRoots) {
			(*iter)->_gc_roots(m_MarkReachableDelegate);
		}
	}

	void GarbageAllocator::update_all_moved() {
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
		StackFrame* frame = get_current_stack_frame();
		while (frame) {
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


	void GarbageAllocator::collect() {
		ASSERT(!m_IsCollecting);
		m_IsCollecting = true;

		// first, do a minor collection
		unmark_heap(m_NurseryHeap);
		mark_all_reachable();
		m_NurseryHeap.compact();
		update_all_moved();

		m_MinorCollectionsSinceLastMajorCollection++;

		if ((int)m_AdultHeap.num_buckets() > m_AdultHeapBucketsThreshold) {
			// fuuuuck, we need a major collection
			unmark_heap(m_AdultHeap);
			mark_all_reachable();
			m_AdultHeap.compact();
			update_all_moved();

			if (m_MinorCollectionsSinceLastMajorCollection == 1) {
				// TODO: Experiment with different growth patterns
				m_AdultHeapBucketsThreshold++;
			}

			m_MinorCollectionsSinceLastMajorCollection = 0;
		}

		m_IsCollecting = false;
	}

	void GarbageAllocator::destruct(GarbageHeader& header, void* object) {
		if (header.free_func) {
			header.free_func(object, header.size);
		}
		if (!(header.flags & GC_FLAG_BLOB) && !(header.flags & GC_FLAG_DESTRUCTED)) {
			IGarbage* gc_object = reinterpret_cast<IGarbage*>(object);
			gc_object->~IGarbage();
		}

		m_Statistics.freed_objects++;
		m_Statistics.freed_size += header.size;
	}

	void GarbageAllocator::pointer_moved(void* from, void* to, size_t size) {
		MovedPointerInfo mpi;
		mpi.old_base = from;
		mpi.new_base = to;
		mpi.size = size;
		m_MovedPointers.push_back(mpi);
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

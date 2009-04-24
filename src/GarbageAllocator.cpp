#include "GarbageAllocator.h"
#include "IGarbage.h"
#include "Handle.h"
#include "lib/Runtime.h"

#include <set>
#include <list>

#define DEBUG_MAX_STACK_DEPTH 50

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
		m_MinorCollectionsSinceLastMajorCollection(0)
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
	
	void GarbageAllocator::mark_reachable(void*& ptr) {
		if (is_object(ptr)) {
			Header* header = find_header(ptr);
			if (header) {
				header->flags |= GC_FLAG_REACHABLE;
			}
			if (!header || !(header->flags & GC_FLAG_BLOB)) {
				IGarbage* gc_object = reinterpret_cast<IGarbage*>(ptr);
				perform_operation(MARK, gc_object);
			}
		}
	}
	
	void GarbageAllocator::update_moved(void*& ptr) {
		if (is_object(ptr)) {
			for each (iter, m_MovedPointers) {
				if (iter->contains(ptr)) {
					void* new_ptr = iter->transform(ptr);
					ptr = new_ptr;
				}
			}

			Header* header = find_header(ptr);
			if (!header || !(header->flags & GC_FLAG_BLOB)) {
				IGarbage* gc_object = reinterpret_cast<IGarbage*>(ptr);
				perform_operation(UPDATE, gc_object);
			}
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
		for each (iter, HandleScope::list()) {
			for each (handle_iter, **iter) {
				mark_reachable((*handle_iter)->value());
			}
		}
		// Snow stack
		StackFrame* frame = get_current_stack_frame();
		while (frame) {
			mark_reachable(reinterpret_cast<void*&>(frame->scope));
			for (size_t i = 0; i < frame->num_temporaries; ++i) {
				mark_reachable(frame->temporaries[i]);
			}
			frame = frame->previous;
		}
		// External roots
		for each (iter, m_ExternalRoots) {
			perform_operation(MARK, *iter);
		}
	}

	void GarbageAllocator::update_all_moved() {
		// C++ stack
		for (auto iter = HandleScope::list().begin(); iter != HandleScope::list().end(); ++iter) {
			for (auto handle_iter = (*iter)->begin(); handle_iter != (*iter)->end(); ++handle_iter) {
				// the object
				update_moved((*handle_iter)->value());
			}
		}
		// Snow stack
		StackFrame* frame = get_current_stack_frame();
		while (frame) {
			update_moved(reinterpret_cast<void*&>(frame->scope));
			for (size_t i = 0; i < frame->num_temporaries; ++i) {
				update_moved(frame->temporaries[i]);
			}
			update_stack_frame(frame, frame->scope);
			frame = frame->previous;
		}
		// External roots
		for each (iter, m_ExternalRoots) {
			perform_operation(UPDATE, *iter);
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
			debug("GC: Running major collection...");
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

	void GarbageAllocator::root_callback(GCOperation op, void*& root) {
#ifdef DEBUG_MAX_STACK_DEPTH
		static int stack_depth = 0;
		stack_depth++;
		ASSERT(stack_depth < DEBUG_MAX_STACK_DEPTH);
#endif
		switch (op) {
			case MARK:
				mark_reachable(root);
				break;
			case UPDATE:
				update_moved(root);
				break;
		}
#ifdef DEBUG_MAX_STACK_DEPTH
		stack_depth--;
#endif
	}

	void GarbageAllocator::perform_operation(GCOperation op, IGarbage* object) {
		if (object->gc_try_lock()) {
			object->_gc_roots(*this, op);
			object->gc_unlock();
		}
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
		// TODO: optimize update_all_moved by using a hashtable for this -- at the cost of range pointers. Do we need those?
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





	void GarbageAllocator::inspect_moved_pointers()
	{
		for each (iter, m_MovedPointers) {
			debug("moved 0x%llx to 0x%llx (size %llu)", iter->old_base, iter->new_base, iter->size);
		}
	}
}

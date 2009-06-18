#include "GarbageAllocator.h"
#include "IGarbage.h"
#include "runtime/Runtime.h"
#include "runtime/Handle.h"
#include "runtime/StackFrame.h"

#include <set>
#include <list>

#define DEBUG_MAX_STACK_DEPTH 50

namespace snow {
	// Constants that can be tweaked
	static const size_t NURSERY_SIZE = (1<<23);     // 1 Mb

	// Must be 0x10 (16), otherwise we can't tell pointers from immediates.
	static const size_t ALIGNMENT = 0x10;
	
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
		// Assume that if m_IsCollecting is true, another thread is collecting garbage.
		if (m_IsCollecting)
		{
			debug("WARNING: Garbage allocation waiting for collection to finish -- hopefully, this allocation was not triggered in the collection process!");
			while (m_IsCollecting) { Garbage::fence(); }
		}

		//ASSERT(!m_IsCollecting && "Cannot perform allocations while collecting garbage!");

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
			auto iter = m_MovedPointers.find(ptr);
			if (iter != m_MovedPointers.end())
			{
				//debug("moving 0x%llx to 0x%llx", ptr, iter->second);
				ptr = iter->second;
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
		#pragma omp parallel sections
		{
			#pragma omp section 
			{
				debug("unmarking nursery heap from thread %d", omp_get_thread_num());
				unmark_heap(m_NurseryHeap);
			}
			#pragma omp section
			{
				debug("unmarking adult heap from thread %d", omp_get_thread_num());
				unmark_heap(m_AdultHeap);
			}
		}
	}

	void GarbageAllocator::mark_all_reachable() {
		#pragma omp parallel sections
		{
			#pragma omp section
			{
				debug("marking c++ stack, on thread %d", omp_get_thread_num());
				// C++ stack
				for (size_t t = 0; t < HandleScope::all_current().num_threads(); ++t)
				{
					HandleScope* handle_scope = HandleScope::all_current().object_for_thread(t);
					while (handle_scope) {
						StackVariable* stack_var = handle_scope->last_variable();
						while (stack_var) {
							mark_reachable(stack_var->value());
							stack_var = stack_var->previous();
						}
						handle_scope = handle_scope->previous();
					}
				}
			}

			#pragma omp section
			{
				debug("marking snow stack, on thread %d", omp_get_thread_num());
				// Snow stack
				for (size_t t = 0; t < get_current_stack_frames().num_threads(); ++t)
				{
					StackFrame* frame = get_current_stack_frames().object_for_thread(t);
					while (frame) {
						mark_reachable(reinterpret_cast<void*&>(frame->scope));
						for (size_t i = 0; i < frame->num_temporaries; ++i) {
							mark_reachable(frame->temporaries[i]);
						}
						frame = frame->previous;
					}
				}
			}

			#pragma omp section
			{
				debug("marking external roots, on thread %d", omp_get_thread_num());
				// External roots
				for each (iter, m_ExternalRoots) {
					perform_operation(MARK, *iter);
				}
			}
		} // omp sections
	}

	void GarbageAllocator::update_all_moved() {
		#pragma omp parallel sections
		{
			#pragma omp section
			{
				debug("updating c++ stack, on thread %d", omp_get_thread_num());
				// C++ stack
				for (size_t t = 0; t < HandleScope::all_current().num_threads(); ++t)
				{
					HandleScope* handle_scope = HandleScope::all_current().object_for_thread(t);
					while (handle_scope) {
						StackVariable* stack_var = handle_scope->last_variable();
						while (stack_var) {
							update_moved(stack_var->value());
							stack_var = stack_var->previous();
						}
						handle_scope = handle_scope->previous();
					}
				}
			}
			#pragma omp section
			{
				debug("updating snow stack, on thread %d", omp_get_thread_num());
				// Snow stack
				for (size_t t = 0; t < get_current_stack_frames().num_threads(); ++t)
				{
					StackFrame* frame = get_current_stack_frames().object_for_thread(t);
					while (frame) {
						update_moved(reinterpret_cast<void*&>(frame->scope));
						for (size_t i = 0; i < frame->num_temporaries; ++i) {
							update_moved(frame->temporaries[i]);
						}
						update_stack_frame(frame, frame->scope);
						frame = frame->previous;
					}
				}
			}
			#pragma omp section
			{
				debug("updating external roots, on thread %d", omp_get_thread_num());
				// External roots
				for each (iter, m_ExternalRoots) {
					perform_operation(UPDATE, *iter);
				}
			}
		} // omp parallel sections

		m_MovedPointers.clear();
	}


	void GarbageAllocator::collect() {
		//ASSERT(!m_IsCollecting);

		// Assume that if we reach this a second time, it's probably because another thread is already collecting.
		if (m_IsCollecting)
		{
			while (m_IsCollecting);
			return;
		}
		
		m_IsCollecting = true;

		Garbage::lock_fence(); // wait for all threads to be ready to be collected

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
		Garbage::unlock_fence();
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
		m_MovedPointers[from] = to;
	}
	
	void GarbageAllocator::register_root(IGarbage* ptr) {
		ASSERT(!contains(ptr));
		if ((uintx)ptr == 0xcdcdcdcdcdcd) TRAP();
		#pragma omp critical(register_root)
		{
			m_ExternalRoots.push_back(ptr);
		}
	}

	void GarbageAllocator::unregister_root(IGarbage* ptr) {
		#pragma omp critical(register_root)
		{
			for (auto iter = m_ExternalRoots.begin(); iter != m_ExternalRoots.end(); ++iter) {
				if (*iter == ptr)
					iter = m_ExternalRoots.erase(iter);
			}
		}
	}





	void GarbageAllocator::inspect_moved_pointers()
	{
		for each (iter, m_MovedPointers) {
			debug("moved 0x%llx to 0x%llx (size %llu)", iter->first, iter->second);
		}
	}
}

#include "GarbageAllocator.h"
#include "IGarbage.h"
#include "Garbage.h"
#include "runtime/Runtime.h"
#include "runtime/Handle.h"
#include "runtime/StackFrame.h"
#include "gc/GarbageTypes.h"
#include "runtime/Scope.h"

#include <set>
#include <list>

#define DEBUG_MAX_STACK_DEPTH 50

namespace snow {
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
		const size_t header_padding = 0x10 - sizeof(GarbageHeader) % 0x10;
		if (contains(ptr)) {
			byte* data = reinterpret_cast<byte*>(ptr);
			Header* header = reinterpret_cast<Header*>(data - ((int)sizeof(Header) + header_padding)); 
			return header;
		}
		return NULL;
	}
	
	bool GarbageAllocator::is_blob(void* ptr) {
		GarbageHeader* header = find_header(ptr);
		if (header) {
			return header->flags & GC_FLAG_BLOB;
		}
		return false;
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
		ASSERT(((uintx)ptr & (ALIGNMENT-1)) == 0);

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
		}
	}
	
	void GarbageAllocator::update_moved(void*& ptr) {
		if (is_object(ptr)) {
			auto iter = m_MovedPointers.find(ptr);
			if (iter != m_MovedPointers.end())
			{
				ptr = iter->second;
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
	
	void GarbageAllocator::with_each_root(GCOperation op, bool update_stack_frames) {
		#pragma omp parallel sections
		{
			#pragma omp section
			{
				// C++ stack
				for (size_t t = 0; t < HandleScope::all_current().num_threads(); ++t)
				{
					HandleScope* scope = HandleScope::all_current().object_for_thread(t);
					while (scope) {
						ValueHandle* handle = scope->last_handle();
						while (handle) {
							root_callback(op, handle->gc_root());
							handle = handle->previous();
						}
						StackProtector* sp = scope->last_stack_protector();
						while (sp) {
							VALUE* data = sp->data();
							for (uintx i = 0; i < sp->length(); ++i) {
								root_callback(op, data[i]);
							}
							sp = sp->previous();
						}
						scope = scope->previous();
					}
				}
			}
			
			#pragma omp section
			{
				// Snow stack
				for (size_t t = 0; t < get_current_stack_frames().num_threads(); ++t)
				{
					StackFrame* frame = get_current_stack_frames().object_for_thread(t);
					while (frame) {
						root_callback(op, frame->scope);
						for (size_t i = 0; i < frame->num_temporaries; ++i) {
							root_callback(op, frame->temporaries[i]);
						}
						if (update_stack_frames)
							update_stack_frame(frame, frame->scope);
						frame = frame->previous;
					}
				}
			}
			
			#pragma omp section
			{
				// External roots
				for each (iter, m_ExternalRoots) {
					root_callback(op, *iter);
				}
			}
		}
	}

	void GarbageAllocator::mark_all_reachable() {
		debug("GC: Marking all roots...");
		with_each_root(MARK);
	}

	void GarbageAllocator::update_all_moved() {
		debug("GC: Updating all roots...");
		with_each_root(UPDATE, true);
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
	
	
	void GarbageAllocator::root_callback(GCOperation op, Value& val) {
		if (val.is_object()) {
			if (is_blob(val.value())) {
				operation(op, val.gc_root());
			} else {
				Ptr<IGarbage>& ptr = reinterpret_cast<Ptr<IGarbage>&>(val);
				root_callback(op, ptr);
			}
		}
	}
	
	void GarbageAllocator::operation(GCOperation op, void*& root) {
		switch (op) {
			case MARK:
				mark_reachable(root);
				break;
			case UPDATE:
				update_moved(root);
				break;
		}
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
	
	void GarbageAllocator::register_root(const Ptr<IGarbage>& ptr) {
		ASSERT(!contains(ptr.value()));
		#pragma omp critical(register_root)
		{
			m_ExternalRoots.push_back(ptr);
		}
	}

	void GarbageAllocator::unregister_root(const Ptr<IGarbage>& ptr) {
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

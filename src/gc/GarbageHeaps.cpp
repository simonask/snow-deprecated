#include "GarbageHeaps.h"
#include "GarbageAllocator.h"

namespace snow {
	const size_t ADULT_HEAP_STANDARD_BUCKET_SIZE = (1<<20);

	//
	// NURSERY HEAP ////////////////////////////////////////
	//

	NurseryHeap::NurseryHeap(GarbageAllocator& allocator, size_t sz) : m_Allocator(allocator), m_Data(NULL), m_Size(sz), m_Offset(0) {}

	void NurseryHeap::initialize() {
		if (!m_Data) {
			m_Data = new(kMalloc) byte[m_Size];
		}
	}

	bool NurseryHeap::enumerator_next(GarbageHeader* current, void* current_object, GarbageHeader*& next_header, void*& next_object) {
		if (!current) {
			if (m_Offset > 0)
			{
				next_header = reinterpret_cast<GarbageHeader*>(m_Data);
				next_object = &m_Data[sizeof(GarbageHeader)];
				return true;
			}
			return false;
		}
		if (contains(current_object)) {
			byte* offset = (byte*)current_object + current->size;
			next_header = reinterpret_cast<GarbageHeader*>(offset);
			next_object = &offset[sizeof(GarbageHeader)];
			if (contains(next_object))
				return true;
			else
				return false;
		}
		return false;
	}

	bool NurseryHeap::contains(const void* ptr) const {
		return &m_Data[sizeof(GarbageHeader)] <= (byte*)ptr && &m_Data[m_Offset] > (byte*)ptr;
	}
		
	void* NurseryHeap::allocate(size_t sz, GarbageAllocator::Header*& header) {
		initialize();
		const size_t header_padding = 0x10 - sizeof(GarbageHeader); // assume sizeof(GarbageHeader) <= 0x10

		size_t required_size = sizeof(GarbageHeader) + header_padding + sz;
		// align
		required_size += (0x10 - required_size % 0x10);
		
		Garbage::fence();
		if (available() < required_size) {
			// Trigger collection!
			m_Allocator.collect();
			ASSERT(available() > required_size);
		}
			
		void* object = NULL;
		#pragma omp critical
		{
			byte* data = &m_Data[m_Offset];
			header = reinterpret_cast<GarbageHeader*>(&data[0]);
			object = reinterpret_cast<void*>(&data[sizeof(GarbageHeader)+header_padding]);
			m_Offset += required_size;
		}
		
		header->size = required_size - (sizeof(GarbageHeader) + header_padding);
		header->flags = GC_NO_FLAGS;
		header->generation = 0;
		header->free_func = NULL;
		
		return object;
	}

	void NurseryHeap::compact() {
		ASSERT(m_NextHeap);

		size_t total_objects = 0;
		size_t moved_objects = 0;

		Enumerator e(*this);
		while (e.next()) {
			++total_objects;
			if (e.header().flags & GC_FLAG_REACHABLE) {
				GarbageHeader* header = NULL;
				void* new_ptr = m_NextHeap->allocate(e.header().size, header);
				header->flags = e.header().flags;
				header->generation = e.header().generation + 1;
				header->free_func = e.header().free_func;

				memcpy(new_ptr, e.object(), e.header().size);

				m_Allocator.pointer_moved(e.object(), new_ptr, e.header().size);

				++moved_objects;

				#ifdef DEBUG
				memset(e.object(), 0xab, e.header().size);
				#endif
			} else {
				m_Allocator.destruct(e.header(), e.object());
				#if DEBUG
				memset(e.object(), 0xef, e.header().size);
				#endif
			}
		}
		
		debug("GC: Compacting nursery heap 0x%llx -- %u of %u objects survived.", this, moved_objects, total_objects);

		m_Offset = 0;
	}


	///
	/// ADULT HEAP ///////////////////////////////////////
	///

	bool AdultHeap::Bucket::contains(const void* _ptr) const {
		const byte* ptr = reinterpret_cast<const byte*>(_ptr);
		return ptr >= &data[sizeof(GarbageHeader)] && ptr < &data[offset];
	}

	bool AdultHeap::contains(const void* ptr) const {
		for (auto iter = m_Buckets.begin(); iter != m_Buckets.end(); ++iter) {
			if (iter->contains(ptr))
				return true;
		}
		return false;
	}
	
	void* AdultHeap::allocate(size_t sz, GarbageHeader*& header) {
		size_t required_size = sizeof(GarbageHeader) + sz;
		// align
		required_size += (0x10 - required_size % 0x10);

		void* object = NULL;

		#pragma omp critical
		{
			std::list<Bucket>::iterator free_bucket_iter = m_Buckets.end();
			for each (iter, m_Buckets) {
				if (iter->available() >= required_size) {
					free_bucket_iter = iter.iterator();
					break;
				}
			}

			if (free_bucket_iter == m_Buckets.end()) {
				// No buckets with free space... Make a new one
				free_bucket_iter = m_Buckets.insert(m_Buckets.begin(), Bucket());
				size_t size = required_size > ADULT_HEAP_STANDARD_BUCKET_SIZE ? required_size : ADULT_HEAP_STANDARD_BUCKET_SIZE;
				free_bucket_iter->data = new(kMalloc) byte[size];
				free_bucket_iter->size = size;
				free_bucket_iter->offset = 0;
			}

			Bucket* bucket = &*free_bucket_iter;

			header = reinterpret_cast<GarbageHeader*>(&bucket->data[bucket->offset]);
			object = &bucket->data[bucket->offset + sizeof(GarbageHeader)];
			bucket->offset += required_size;
		}

		header->size = required_size - sizeof(GarbageHeader);
		header->flags = GC_NO_FLAGS;
		header->generation = 0;
		header->free_func = NULL;

		return object;
	}

	void AdultHeap::compact() {
		// TODO: Some heuristics about whether or not to do this at all, since it's pretty expensive.

		std::list<Bucket> old_buckets = m_Buckets;
		m_Buckets = std::list<Bucket>();

		for each (iter, old_buckets) {
			size_t offset = 0;
			while (offset < iter->offset) {
				GarbageHeader* old_header = reinterpret_cast<GarbageHeader*>(&iter->data[offset]);
				void* old_object = &iter->data[offset + sizeof(GarbageHeader)];

				if (old_header->flags & GC_FLAG_REACHABLE) {
					GarbageHeader* new_header;
					void* new_object = allocate(old_header->size, new_header);
					new_header->generation = old_header->generation + 1;
					new_header->flags = old_header->flags;
					new_header->free_func = old_header->free_func;

					memcpy(new_object, old_object, old_header->size);

					m_Allocator.pointer_moved(old_object, new_object, old_header->size);
				} else {
					m_Allocator.destruct(*old_header, old_object);
				}
				
				offset += sizeof(GarbageHeader) + old_header->size;
			}
			delete [] iter->data;
		}
	}

	bool AdultHeap::enumerator_next(GarbageHeader* current, void* current_object, GarbageHeader*& next_header, void*& next_object) {
		// TODO: Make this easier to do efficiently...

		if (m_Buckets.begin() == m_Buckets.end())
			return false;

		if (!current) {
			auto iter = m_Buckets.begin();
			next_header = reinterpret_cast<GarbageHeader*>(iter->data);
			next_object = &iter->data[sizeof(GarbageHeader)];
			return true;
		}

		std::list<Bucket>::iterator current_bucket_iter;
		for each (iter, m_Buckets) {
			if (iter->contains(current_object)) {
				current_bucket_iter = iter.iterator();
				break;
			}
		}
		ASSERT(current_bucket_iter != m_Buckets.end());
		std::list<Bucket>::iterator next_bucket_iter = current_bucket_iter;
		++next_bucket_iter;

		Bucket* current_bucket = &*current_bucket_iter;

		// try next object within current bucket
		next_header = reinterpret_cast<GarbageHeader*>((byte*)current_object + current->size);
		next_object = reinterpret_cast<byte*>(next_header) + sizeof(GarbageHeader);

		if (!current_bucket->contains(next_object)) {
			if (next_bucket_iter == m_Buckets.end())
				return false;
			else {
				Bucket* next_bucket = &*next_bucket_iter;
				next_header = reinterpret_cast<GarbageHeader*>(next_bucket->data);
				next_object = &next_bucket->data[sizeof(GarbageHeader)];
				return true;
			}
		}

		return true;
	}
}

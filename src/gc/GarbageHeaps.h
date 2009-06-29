#ifndef GARBAGEHEAPS_H_SFHKF56
#define GARBAGEHEAPS_H_SFHKF56

#include "base/Basic.h"
#include <list>
#include "gc/GarbageTypes.h"

namespace snow {
	struct GarbageHeader;
	class GarbageAllocator;

	class IGarbageHeap {
	public:
		virtual bool contains(const void*) const = 0;
		virtual void* allocate(size_t sz, GarbageHeader*&) = 0;
		virtual void compact() = 0;
		
		virtual bool enumerator_next(GarbageHeader* current, void* current_object, GarbageHeader*& next_header, void*& next_object) = 0;

		class Enumerator {
		private:
			IGarbageHeap& m_Heap;
			GarbageHeader* m_Header;
			void* m_Object;
		public:
			Enumerator(IGarbageHeap& heap) : m_Heap(heap), m_Header(NULL), m_Object(NULL) {}
			bool next() { return m_Heap.enumerator_next(m_Header, m_Object, m_Header, m_Object); }
			GarbageHeader& header() const { return *m_Header; }
			void* object() const { return m_Object; }
		};
	};

	class NurseryHeap : public IGarbageHeap {
	private:
		GarbageAllocator& m_Allocator;
		byte* m_Data;
		size_t m_Size;
		size_t m_Offset;
		IGarbageHeap* m_NextHeap;
		
		void initialize();
	public:
		NurseryHeap(GarbageAllocator&, size_t size);

		IGarbageHeap* next_heap() const { return m_NextHeap; }
		void set_next_heap(IGarbageHeap* heap) { m_NextHeap = heap; }
		
		size_t size() const { return m_Size; }
		size_t available() const { return m_Size - m_Offset; }
		size_t offset() const { return m_Offset; }

		// IGarbageHeap impl
		bool contains(const void* ptr) const;
		void* allocate(size_t sz, GarbageHeader*& header);
		void compact();
		bool enumerator_next(GarbageHeader* current, void* object, GarbageHeader*& next_header, void*& next_object);
	};

	class AdultHeap : public IGarbageHeap {
	private:
		GarbageAllocator& m_Allocator;

		struct Bucket {
			byte* data;
			size_t size;
			size_t offset;

			bool contains(const void* ptr) const;
			size_t available() const { return size - offset; }
		};

		std::list<Bucket> m_Buckets;
	public:
		AdultHeap(GarbageAllocator& allocator) : m_Allocator(allocator) { }

		size_t num_buckets() const { return m_Buckets.size(); }
		// IGarbageHeap impl
		bool contains(const void*) const;
		void* allocate(size_t sz, GarbageHeader*&);
		void compact();
		bool enumerator_next(GarbageHeader* current, void* object, GarbageHeader*& next_header, void*& next_object);	
	};
}

#endif // GARBAGEHEAPS_H_SFHKF56


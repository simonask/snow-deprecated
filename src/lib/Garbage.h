#ifndef GARBAGE_H_N12SKHS0
#define GARBAGE_H_N12SKHS0

#include "Basic.h"

namespace snow {
	struct StackFrame;
	
	class Garbage {
		friend class MemoryHeap;
	private:
		enum Flag {
			NO_FLAGS   = 0,
			MARKED     = 1,
			BLOB       = 1 << 1
		};
		
		struct Header {
			size_t size;
			uint32_t flags;
			uint32_t generation;
		};
		
		static void mark_frame(StackFrame*);
		
		Header* gc_header() const { return header(this); };
		static Header* header(const void* ptr);
		
		static void _mark(void* ptr);
		static void _unmark(void* ptr);
	public:
		Garbage() {}
		virtual ~Garbage() {}
		
		inline bool gc_is_managed() const { return gc_header() != NULL; }
	
		virtual void gc_mark();
		virtual void gc_unmark();
		virtual bool gc_is_marked();
		
		static void* alloc(size_t sz, bool blob = false);
		static void collect();
		static void mark(void* ptr);
		static void unmark(void* ptr);
		static bool is_marked(void* ptr);
		static bool is_blob(void* ptr);
		
		void* operator new(size_t sz) { return alloc(sz); };
		void operator delete(void* ptr) { unmark(ptr); };
		
		class Lock {
		private:
			bool m_Locked;
		public:
			Lock() : m_Locked(false) {}
			
			inline bool lock() {
				if (m_Locked)
					return false;
				m_Locked = true;
				return true;
			}
			
			inline void unlock() {
				m_Locked = false;
			}
			
			inline bool is_locked() const { return m_Locked; }
		};
		
		static struct Stats {
			size_t allocated_size;
			size_t allocated_objects;
			size_t collected_size;
			size_t collected_objects;
			size_t minor_collections;
			size_t major_collections;
			
			Stats() { memset(this, '\0', sizeof(*this)); }
		} stats;
	};
	
	inline void Garbage::gc_mark() {
		Garbage::_mark(this);
	}
	
	inline void Garbage::gc_unmark() {
		Garbage::_unmark(this);
	}
	
	inline bool Garbage::gc_is_marked() {
		return Garbage::is_marked(this);
	}
}

#endif /* end of include guard: GARBAGE_H_N12SKHS0 */

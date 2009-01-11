#ifndef REFPTR_H_8DZ2IBFQ
#define REFPTR_H_8DZ2IBFQ

#include <stdlib.h>
#include <type_traits>
#include "Util.h"

namespace snow {
	#ifdef DEBUG
	inline bool is_stack_pointer(void* ptr) {
		// XXX: WARNING! Highly non-portable, and probably not even correct
		// on OS X.
		uint64_t mask = 0x7fff00000000;
		return ((uint64_t)ptr & mask) == mask;
	}
	#endif
	
	class RefCounter {
	private:
		void* m_Ptr;
		int64_t m_Count;
	public:
		static RefCounter Null;
		
		explicit RefCounter(void* ptr) : m_Ptr(ptr), m_Count(0) {
			#ifdef DEBUG
			if (is_stack_pointer(ptr))
				warn("Creating a RefPtr to a potential stack pointer. Expect a crash nearby...");
			#endif
		}
		~RefCounter() { }
		void retain() { m_Count++; }
		void release() {
			int64_t old = m_Count;
			m_Count--;
			if (m_Count > old)
				m_Count = 0;
		}
		template <typename T>
		T* ptr() const { return static_cast<T*>(m_Ptr); }
		int count() const { return m_Count; }
	};
	
	
	template <typename T>
	class RefPtr {
	template <typename U> friend class RefPtr;
	private:
		RefCounter* m_Counter;
		inline void init(T* ptr) {
			m_Counter = ptr ? new RefCounter(ptr) : &RefCounter::Null;
		}
		inline void retain() { m_Counter->retain(); }
		inline void release() {
			m_Counter->release();
			if (m_Counter->count() == 0) {
				delete m_Counter->ptr<T>();
				if (m_Counter != &RefCounter::Null)
					delete m_Counter;
			}
		}
	public:
		RefPtr(T* ptr = NULL) {
			init(ptr);
			retain();
		}
		template <typename U>
		RefPtr(const RefPtr<U>& other) : m_Counter(other.m_Counter) {
			static_assert(std::is_base_of<T, U>::value, "invalid cast");
			retain();
		}
		RefPtr<T>(const RefPtr<T>& other) : m_Counter(other.m_Counter) {
			retain();
		}
		~RefPtr() { release(); }
		
		template <typename U>
		RefPtr<T>& operator=(const RefPtr<U>& other) {
			static_assert(std::is_base_of<T, U>::value, "invalid cast");
			release();
			m_Counter = other.m_Counter;
			retain();
			return *this;
		}
		template <typename U>
		RefPtr<T>& operator=(U* ptr) {
			static_assert(std::is_base_of<T, U>::value, "invalid cast");
			release();
			init(ptr);
			retain();
			return *this;
		}
		
		template <typename U>
		U* as() const { return dynamic_cast<U*>(m_Counter->ptr<T>()); }
		
		T* operator->() const { return m_Counter->ptr<T>(); }
		T& operator*() const { return *m_Counter->ptr<T>(); }
		int ref_count() const { return m_Counter->count(); }
		
		operator bool() const { return m_Counter->ptr<T>(); }
	};
}

#endif /* end of include guard: REFPTR_H_8DZ2IBFQ */

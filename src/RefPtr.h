#ifndef REFPTR_H_8DZ2IBFQ
#define REFPTR_H_8DZ2IBFQ

#include <stdlib.h>
#include <type_traits>
#include "Util.h"

namespace snow {
	class RefCounter {
	private:
		void* m_Ptr;
		int64_t m_Count;
	public:
		explicit RefCounter(void* ptr) : m_Ptr(ptr), m_Count(0) {}
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
			m_Counter = ptr ? new RefCounter(ptr) : NULL;
		}
		inline void retain() { if (m_Counter) m_Counter->retain(); }
		inline void release() {
			if (m_Counter) {
				m_Counter->release();
				if (m_Counter->count() == 0) {
					delete m_Counter->ptr<T>();
				}
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
		
		T* operator->() const { return m_Counter ? m_Counter->ptr<T>() : NULL; }
		T& operator*() const { if (!m_Counter) TRAP("reference to NULL"); return *m_Counter->ptr<T>(); }
		int ref_count() const { return m_Counter ? m_Counter->count() : 0; }
		
		operator bool() const { return m_Counter && m_Counter->ptr<T>(); }
	};
}

#endif /* end of include guard: REFPTR_H_8DZ2IBFQ */

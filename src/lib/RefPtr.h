#ifndef REFPTR_H_8DZ2IBFQ
#define REFPTR_H_8DZ2IBFQ

namespace snot {
	template <typename T>
	class RefPtr {
	private:
		class Counter {
		private:
			T* m_Ptr;
			int m_Count;
		public:
			Counter(T* ptr) : m_Ptr(ptr), m_Count(0) {}
			~Counter() { delete m_Ptr; }
			void inc() { ++m_Count; }
			void dec() { --m_Count; if (m_Count <= 0) delete this; }
			T* ptr() const { return m_Ptr; }
			int count() const { return m_Count; }
		};
		
		Counter* m_Counter;
	public:
		RefPtr(T* ptr = NULL) : m_Counter(new Counter(ptr)) { m_Counter->inc(); }
		RefPtr(const RefPtr<T>& other) : m_Counter(other.m_Counter) { m_Counter->inc(); }
		~RefPtr() { m_Counter->dec(); }
		
		RefPtr& operator=(const RefPtr<T>& other) {
			m_Counter->dec();
			m_Counter = other.m_Counter;
			m_Counter->inc();
		}
		
		T* operator->() const { return m_Counter->ptr(); }
		T& operator*() const { return *m_Counter->ptr(); }
		int count() const { return m_Counter->count(); }
	};
}

#endif /* end of include guard: REFPTR_H_8DZ2IBFQ */

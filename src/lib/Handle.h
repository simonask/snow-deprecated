#ifndef HANDLE_H_M327QUKJ
#define HANDLE_H_M327QUKJ

namespace snot {
	template <class T>
	class Handle {
	private:
		T* m_Ptr;
	public:
		Handle(const T& obj) { m_Ptr = new T(obj); }
		Handle(const Handle<T>& other) { m_Ptr = new T(*other.m_Ptr); }
		~Handle() { delete m_Ptr; }
		Handle<T>& operator=(const Handle<T>& other) {
			delete m_Ptr;
			m_Ptr = new T(*other.m_Ptr);
		}
		T* operator->() { return m_Ptr; }
		const T* operator->() const { return m_Ptr; }
	};
}

#endif /* end of include guard: HANDLE_H_M327QUKJ */

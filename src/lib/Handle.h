#ifndef HANDLE_H_3UB7BMPV
#define HANDLE_H_3UB7BMPV

#include "Value.h"
#include "Basic.h"

namespace snow {
	class ValueHandle {
	private:
		VALUE m_Value;
		ValueHandle* m_Previous;
		ValueHandle* m_Next;
	public:
		ValueHandle(VALUE val = NULL);
		virtual ~ValueHandle();
		
		VALUE value() const { return m_Value; }
		operator VALUE() const { return m_Value; }
		ValueHandle& operator=(VALUE val) { m_Value = val; return *this; }
		operator bool() const { return m_Value; }
		bool operator==(const VALUE val) const { return m_Value == val; }
		
		ValueHandle* previous() const { return m_Previous; }
		void set_previous(ValueHandle*);
		ValueHandle* next() const { return m_Next; }
		void set_next(ValueHandle*);
		
		static ValueHandle* last();
		
		bool is_object() const { return snow::is_object(m_Value); }
		template <typename T>
		T* cast() const { return static_cast<T*>(m_Value); }
		template <typename T>
		bool is_a() const { return object_cast<T>() != NULL; }
	};
	
	inline VALUE value(const ValueHandle& h) { return h.value(); }
	
	template <typename T>
	class Handle : public ValueHandle {
	public:
		Handle(T* val = NULL) : ValueHandle(val) {}
		
		operator T*() const { return cast<T>(); }
		T* operator->() const { return cast<T>(); }
		T& operator*() const { return *cast<T>(); }
	};
}

#endif /* end of include guard: HANDLE_H_3UB7BMPV */

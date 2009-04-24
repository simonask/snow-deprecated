#ifndef HANDLE_H_3UB7BMPV
#define HANDLE_H_3UB7BMPV

#include "lib/Value.h"
#include "Basic.h"

#include <list>

namespace snow {
	class HandleScope;

	class ValueHandle {
	public:
		typedef std::list<ValueHandle*> List;
	private:
		VALUE m_Value;
		HandleScope* m_Scope;
	protected:
		void set_permanent();
	public:
		ValueHandle(VALUE val = NULL);
		// XXX: Due to a bug in GCC (or??) we still need the copy-constructor.
		ValueHandle(const ValueHandle& other);
		virtual ~ValueHandle();
		
		VALUE& value() { return m_Value; }
		VALUE value() const { return m_Value; }
		operator VALUE() const { return m_Value; }
		ValueHandle& operator=(VALUE val) { m_Value = val; return *this; }
		operator bool() const { return m_Value; }
		bool operator==(const VALUE val) const { return m_Value == val; }
		
		bool is_object() const { return snow::is_object(m_Value); }
		template <typename T>
		T* cast() const { return static_cast<T*>(m_Value); }
		template <typename T>
		bool is_a() const { return object_cast<T>() != NULL; }
		
		VALUE* value_ptr() { return &m_Value; }
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
	
	/// HandleScope

	class HandleScope
	{
	public:
		typedef std::list<HandleScope*> List;
		typedef ValueHandle::List::iterator iterator;
		typedef ValueHandle::List::value_type value_type;
	private:
		List::iterator m_Iterator;
		ValueHandle::List m_Handles;
	public:
		HandleScope();
		~HandleScope();

		ValueHandle::List::iterator add(ValueHandle*);
		void remove(ValueHandle*);

		iterator begin() { return m_Handles.begin(); }
		iterator end() { return m_Handles.end(); }

		static List& list();
	};
}

#endif /* end of include guard: HANDLE_H_3UB7BMPV */

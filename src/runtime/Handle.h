#ifndef HANDLE_H_3UB7BMPV
#define HANDLE_H_3UB7BMPV

#include "runtime/Value.h"
#include "base/Basic.h"

#include <list>

namespace snow {
	class HandleScope;

	class StackVariable {
		friend class HandleScope;
	private:
		static VALUE s_Null;
		StackVariable* m_Previous;
	public:
		StackVariable();
		virtual ~StackVariable();

		StackVariable* previous() const { return m_Previous; }

		// Dummy accessor used by the GC. Overridden by ValueHandle.
		virtual VALUE& value() { return s_Null; }
		virtual VALUE value() const { return s_Null; }
	};

	class ValueHandle : private StackVariable {
	private:
		VALUE m_Value;
	public:
		ValueHandle(VALUE val = NULL) : m_Value(val) {}
		
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

	template <typename T>
	class Local : private StackVariable {
	private:
		T m_Object;
	public:
		template <typename... A>
		Local(A... args) : m_Object(args...) { }
		virtual ~Local() { }
		T& object() { return m_Object; }
		const T& object() const { return m_Object; }
		operator T&() { return m_Object; }
		operator const T&() const { return m_Object; }
	};

#define L(local) (local.object())
	
	/// HandleScope

	class HandleScope
	{
	private:
		HandleScope* m_Previous;
		bool m_Destructing;
		StackVariable* m_LastVariable;

		static void set_current(HandleScope*);
	public:
		HandleScope();
		~HandleScope();

		void add(StackVariable*);
		void remove(StackVariable*);

		StackVariable* last_variable() const { return m_LastVariable; }
		HandleScope* previous() const { return m_Previous; }

		static HandleScope* current();
	};


	/// Local impl
	inline StackVariable::StackVariable() : m_Previous(NULL) {
		HandleScope::current()->add(this);
	}
	inline StackVariable::~StackVariable() {
		HandleScope::current()->remove(this);
	}
}

#endif /* end of include guard: HANDLE_H_3UB7BMPV */

#ifndef HANDLE_H_3UB7BMPV
#define HANDLE_H_3UB7BMPV

#include "runtime/Value.h"
#include "base/Basic.h"
#include "base/ThreadLocal.h"

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


	template <typename T>
	class HandleSmartPointerMixin {
	protected:
		virtual T* raw_pointer() const = 0;
	public:
		operator T*() const { return raw_pointer(); }
		T* operator->() const { return raw_pointer(); }
		T& operator*() const { return *raw_pointer(); }
	};
	template <>
	class HandleSmartPointerMixin<void> {};

	template <typename T>
	class Handle : public StackVariable, public HandleSmartPointerMixin<T> {
	private:
		T* m_Value;

		T* raw_pointer() const { return m_Value; }
	public:
		Handle(T* val = NULL) : m_Value(val) {}
		VALUE& value() { return reinterpret_cast<VALUE&>(m_Value); }
		VALUE value() const { return m_Value; }
		operator VALUE() const { return m_Value; }
		Handle<T>& operator=(VALUE val) { m_Value = object_cast<T>(val); return *this; }
		operator bool() const { return m_Value; }

		bool operator==(VALUE other) { return m_Value == other; }
		bool is_object() const { return snow::is_object(m_Value); }
		template <typename U>
		U* cast() const { return object_cast<U>(m_Value); }
		template <typename U>
		bool is_a() const { return cast<U>() != NULL; }

		VALUE* value_ptr() { return &reinterpret_cast<VALUE>(m_Value); }
	};

	typedef Handle<void> ValueHandle;

	template <typename T, typename IsSmartPointer>
	inline VALUE value(const Handle<T>& h) { return h.value(); }
	
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
		static ThreadLocal<HandleScope*>& all_current();
	};


	/// Local impl
	inline StackVariable::StackVariable() : m_Previous(NULL) {
		ASSERT(HandleScope::current() != NULL && "No handle scope!");
		HandleScope::current()->add(this);
	}
	inline StackVariable::~StackVariable() {
		ASSERT(HandleScope::current() != NULL && "No handle scope!");
		HandleScope::current()->remove(this);
	}
}

#endif /* end of include guard: HANDLE_H_3UB7BMPV */

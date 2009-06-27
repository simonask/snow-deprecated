#ifndef HANDLE_H_3UB7BMPV
#define HANDLE_H_3UB7BMPV

#include "runtime/Value.h"
#include "base/Basic.h"
#include "base/ThreadLocal.h"

#include <list>
#include <type_traits>

namespace snow {
	class HandleScope;
	
	// Common smart pointer functions, POD or non-POD
	template <typename T>
	class BasicPtr {
	protected:
		T* m_Value;
	public:
		BasicPtr(T* val = NULL) : m_Value(val) {}
		BasicPtr<T>& operator=(const BasicPtr<T>& other) { m_Value = other.m_Value; return *this; }
		BasicPtr<T>& operator=(T* val) { m_Value = val; return *this; }
		T* value() const { return m_Value; }
		T** value_ptr() { return &m_Value; }
		operator bool() const { return m_Value != NULL; }
		
		bool operator==(const BasicPtr<T>& other) const { return m_Value == other.m_Value; }
		bool operator!=(const BasicPtr<T>& other) const { return m_Value != other.m_Value; }
		bool operator==(const T* ptr) const { return m_Value == ptr; }
		bool operator!=(const T* ptr) const { return m_Value != ptr; }
	};
	
	class Value : public BasicPtr<void> {
	public:
		Value(VALUE val = NULL) : BasicPtr<void>(val) {}
		
		bool is_integer() const { return snow::is_integer(value()); }
		bool is_object() const { return snow::is_object(value()); }
		bool is_true() const { return snow::is_true(value()); }
		bool is_false() const { return snow::is_false(value()); }
		bool is_boolean() const { return snow::is_boolean(value()); }
		bool is_nil() const { return snow::is_nil(value()); }
		bool is_symbol() const { return snow::is_symbol(value()); }
		bool is_numeric() const { return snow::is_numeric(value()); }
		bool is_float() const { return snow::is_float(value()); }
	};
	
	inline bool operator==(VALUE a, const Value& b) { return b == a; }
	inline bool operator!=(VALUE a, const Value& b) { return b != a; }
	
	// POD smart pointer
	template <typename T>
	class DataPtr : public BasicPtr<T> {
	public:
		DataPtr(T* val = NULL) : BasicPtr<T>(val) {}
		T& operator[](uintx idx) const { return this->m_Value[idx]; }
		operator T*() const { return this->m_Value; }
	};
	// Non-POD smart pointer
	template <typename T>
	class Ptr : public BasicPtr<T> {
	public:
		struct Null {};
		
		Ptr(Null* null = NULL) : BasicPtr<T>(NULL) {}
		template <typename U>
		Ptr(U* val) : BasicPtr<T>(val) {}
		template <typename U>
		Ptr(const Ptr<U>& other) : BasicPtr<T>(other.value()) {}
		
		T* operator->() const { return this->m_Value; }
		T& operator*() const { return *this->m_Value; }
		
		operator Value&() { return *reinterpret_cast<Value*>(this); }
		operator const Value&() const { return *reinterpret_cast<const Value*>(this); }
		bool is_object() const { return snow::is_object(this->m_Value); }
	};
	
	// The following two templated structs are for selecting the appropriate base of
	// the general Handle<T> smart pointer, without running into weird C++ typedef rules.
	template <typename T, bool IsPodType> struct SelectSmartPointerType;
	template <typename T>
	struct SelectSmartPointerType<T, false> {
		typedef Ptr<T> PointerType;
	};
	template <typename T>
	struct SelectSmartPointerType<T, true> {
		typedef DataPtr<T> PointerType;
	};
	template <>
	struct SelectSmartPointerType<void, false> {
		typedef Value PointerType;
	};
	template <typename T>
	struct HandleType {
		typedef SelectSmartPointerType<T, std::is_pod<T>::value> BaseSelector;
		typedef typename BaseSelector::PointerType Base;
	};
	
	// The general Handle smart pointer, used by the GC.
	template <typename T>
	class Handle : public HandleType<T>::Base {
		friend class HandleScope;
	private:
		Handle<void>* m_Previous;
		
		void add();
		void remove();
	public:
		typedef typename HandleType<T>::Base Base;
		
		Handle(T* val = NULL) : Base(val) { add(); }
		Handle(const Base& other) : Base(other.value()) { add(); }
		~Handle() { remove(); }
		operator VALUE() const { return this->value(); }
		
		Handle<void>* previous() const { return m_Previous; }
	};

	// Can contain any value.
	typedef Handle<void> ValueHandle;

	template <typename T>
	inline VALUE value(const BasicPtr<T>& h) { return h.value(); }
	
	class BasicLocal {
		friend class HandleScope;
	private:
		BasicLocal* m_Previous;
		
		void add();
		void remove();
	public:
		BasicLocal() : m_Previous(NULL) { add(); }
		virtual ~BasicLocal() { remove(); }
	};
	
	template <typename T>
	class Local : public BasicLocal {
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
		Handle<void>* m_LastHandle;
		BasicLocal* m_LastLocal;

		static void set_current(HandleScope*);
	public:
		HandleScope();
		~HandleScope();

		void add(BasicLocal*);
		void remove(BasicLocal*);
		void add(Handle<void>*);
		void remove(Handle<void>*);

		Handle<void>* last_handle() const { return m_LastHandle; }
		BasicLocal* last_variable() const { return m_LastLocal; }
		HandleScope* previous() const { return m_Previous; }

		static HandleScope* current();
		static ThreadLocal<HandleScope*>& all_current();
	};

	template <typename T>
	inline void Handle<T>::add() {
		HandleScope::current()->add(reinterpret_cast<Handle<void>*>(this));
	}
	template <typename T>
	inline void Handle<T>::remove() {
		HandleScope::current()->remove(reinterpret_cast<Handle<void>*>(this));
	}

	inline void BasicLocal::add() {
		HandleScope::current()->add(this);
	}
	inline void BasicLocal::remove() {
		HandleScope::current()->remove(this);
	}
}

#endif /* end of include guard: HANDLE_H_3UB7BMPV */

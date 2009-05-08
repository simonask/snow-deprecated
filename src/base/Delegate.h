#ifndef DELEGATE_H_FDH43HJ
#define DELEGATE_H_FDH43HJ

#include "RefPtr.h"

namespace snow {
	template <typename Ret, typename... Args>
	class DelegateImpl {
	public:
		virtual Ret invoke(Args...) = 0;
	};

	template <typename Ret, typename... Args>
	class Delegate {
	private:
		RefPtr<DelegateImpl<Ret, Args...>> m_Impl;
	public:
		Delegate(RefPtr<DelegateImpl<Ret, Args...>> impl) : m_Impl(impl) {}
		Ret invoke(Args... a) { return m_Impl->invoke(a...); }
		Ret operator()(Args... a) { return invoke(a...); }
	};

	template <typename Ret, typename... Args>
	class StaticDelegate : public DelegateImpl<Ret, Args...> {
	public:
		typedef Ret(*FuncType)(Args...);
	private:
		FuncType m_Func;
	public:
		StaticDelegate(FuncType func) : m_Func(func) {}
		Ret invoke(Args... a) { return m_Func(a...); }
	};

	template <typename Instance, typename Ret, typename... Args>
	class MemberDelegate : public DelegateImpl<Ret, Args...> {
	public:
		typedef Ret(Instance::*FuncType)(Args...);
	private:
		Instance* m_Instance;
		FuncType m_Func;
	public:
		MemberDelegate(Instance* inst, FuncType func) : m_Instance(inst), m_Func(func) {}
		Ret invoke(Args... a) { return (m_Instance->*m_Func)(a...); }
	};

	template <typename Ret, typename... Args>
	Delegate<Ret, Args...> create_delegate(Ret(*func)(Args...)) {
		return Delegate<Ret, Args...>(new StaticDelegate<Ret, Args...>(func));
	}

	template <typename Instance, typename Ret, typename... Args>
	Delegate<Ret, Args...> create_delegate(Instance* instance, Ret(Instance::*func)(Args...)) {
		return Delegate<Ret, Args...>(new MemberDelegate<Instance, Ret, Args...>(instance, func));
	}
}

#endif // DELEGATE_H_FDH43HJ


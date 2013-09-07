#ifndef ClousesImpl_h__
#define ClousesImpl_h__

#include "utils.h"
#include "MethodMock.h"

template <typename R, typename... arglist>
struct FunctionWhenClouseImpl :
	public FirstFunctionWhenClouse<R, arglist...>,
	public NextFunctionWhenClouse<R, arglist...>
{

	FunctionWhenClouseImpl(InvocationMockBase<R, arglist...> * invocationMock) :
		invocationMock(invocationMock)
	{
		ThenReturn(R{});
	}

	~FunctionWhenClouseImpl(){}

	NextFunctionWhenClouse<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
		auto doMock = new DoMock<R, arglist...>(method);
		invocationMock->append(doMock);
		return *this;
	}

	NextFunctionWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) override {
		auto doMock = new DoMock<R, arglist...>(method);
		invocationMock->clear();
		invocationMock->append(doMock);
		return *this;
	}
	
private:
	InvocationMockBase<R, arglist...>* invocationMock;
};

template <typename R, typename... arglist>
struct StubFunctionClouseImpl : public StubFunctionClouse<R, arglist...> {
	StubFunctionClouseImpl(MethodMock<R, arglist...>* methodMock) : methodMock(methodMock) {
	}

	FirstFunctionWhenClouse<R, arglist...>& When(const arglist&... args) override {
		InvocationMockBase<R, arglist...> * invocationMock = methodMock->append(args...);
		FunctionWhenClouseImpl<R, arglist...> * whenClouse = new FunctionWhenClouseImpl<R, arglist...>
			(invocationMock);
		return *whenClouse;
	}

	NextFunctionWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) override {
		FunctionWhenClouseImpl<R, arglist...> * whenClouse = new FunctionWhenClouseImpl<R, arglist...>(methodMock->last());
		whenClouse->Do(method);
		return *whenClouse;
	}

private:
	MethodMock<R, arglist...>* methodMock;
};


template <typename... arglist>
struct ProcedureWhenClouseImpl :
	public FirstProcedureWhenClouse<arglist...>,
	public NextProcedureWhenClouse<arglist...>{

		ProcedureWhenClouseImpl(InvocationMockBase<void, arglist...>* invocationMock) :
			invocationMock(invocationMock)
		{
			ThenReturn();
		}

		virtual ~ProcedureWhenClouseImpl() {}

		NextProcedureWhenClouse<arglist...>& ThenDo(std::function<void(arglist...)> method) override {
			auto doMock = new DoMock<void, arglist...>(method);
			invocationMock->append(doMock);
			return *this;
		}

		NextProcedureWhenClouse<arglist...>& Do(std::function<void(arglist...)> method) override {
			auto doMock = new DoMock<void, arglist...>(method);
			invocationMock->clear();
			invocationMock->append(doMock);
			return *this;
		}
		
private:
	InvocationMockBase<void, arglist...>* invocationMock;
};

template <typename... arglist>
struct StubProcedureClouseImpl : public StubProcedureClouse<arglist...> {
	StubProcedureClouseImpl(MethodMock<void, arglist...>* methodMock) : methodMock(methodMock) {
	}

	FirstProcedureWhenClouse<arglist...>& When(const arglist&... args) override {
		InvocationMockBase<void, arglist...> * invocationMock = methodMock->append(args...);
		ProcedureWhenClouseImpl<arglist...> * whenClouse = new ProcedureWhenClouseImpl<arglist...>(invocationMock);
		return *whenClouse;
	};

	NextProcedureWhenClouse<arglist...>& Do(std::function<void(arglist...)> method) override {
		ProcedureWhenClouseImpl<arglist...> * whenClouse = new ProcedureWhenClouseImpl<arglist...>(methodMock->last());
		whenClouse->Do(method);
		return *whenClouse;
	}
	
private:
	MethodMock<void, arglist...>* methodMock;
};
#endif // ClousesImpl_h__
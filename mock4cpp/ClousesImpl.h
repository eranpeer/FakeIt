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
		invocationMock->append(new DefaultReturnMock<R>());
	}

	~FunctionWhenClouseImpl(){}

	NextFunctionWhenClouse<R, arglist...>& ThenReturn(R r) override {
		ReturnMock<R>* returnMock = new ReturnMock<R>(r);
		invocationMock->append(returnMock);
		return *this;
	};

	NextFunctionWhenClouse<R, arglist...>& ThenThrow() override {
		ThrowMock<R>* throwMock = new ThrowMock<R>();
		invocationMock->append(throwMock);
		return *this;
	};

	NextFunctionWhenClouse<R, arglist...>& Return(R r) override {
		ReturnMock<R>* returnMock = new ReturnMock<R>(r);
		invocationMock->clear();
		invocationMock->append(returnMock);
		return *this;
	};

	NextFunctionWhenClouse<R, arglist...>& Throw() override {
		ThrowMock<R>* throwMock = new ThrowMock<R>();
		invocationMock->clear();
		invocationMock->append(throwMock);
		return *this;
	};

private:
	InvocationMockBase<R, arglist...>* invocationMock;
};

template <typename R, typename... arglist>
struct StubFunctionClouseImpl : public StubFunctionClouse<R, arglist...> {
	StubFunctionClouseImpl(MethodMock<R, arglist...>* methodMock) : methodMock(methodMock) {
	}

	FirstFunctionWhenClouse<R, arglist...>& When(arglist... args) override {
		InvocationMock<R, arglist...> * invocationMock = new InvocationMock<R, arglist...>(args...);
		FunctionWhenClouseImpl<R, arglist...> * whenClouse = new FunctionWhenClouseImpl<R, arglist...>
			(invocationMock);
		methodMock->clear();
		methodMock->append(invocationMock);
		return *whenClouse;
	}

	NextFunctionWhenClouse<R, arglist...>& Return(R r)  override  {
		FunctionWhenClouseImpl<R, arglist...> * whenClouse = new FunctionWhenClouseImpl<R, arglist...>(
			methodMock->first());
		whenClouse->Return(r);
		return *whenClouse;
	}

	NextFunctionWhenClouse<R, arglist...>& Throw()  override {
		FunctionWhenClouseImpl<R, arglist...> * whenClouse = new FunctionWhenClouseImpl<R, arglist...>(
			methodMock->first());
		whenClouse->Throw();
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
			invocationMock->append(new VoidMock());
		}

		virtual ~ProcedureWhenClouseImpl() {}

		NextProcedureWhenClouse<arglist...>& ThenReturn() {
			VoidMock* voidMock = new VoidMock();
			invocationMock->append(voidMock);
			return *this;
		};

		NextProcedureWhenClouse<arglist...>& ThenThrow() {
			ThrowMock<void>* throwMock = new ThrowMock<void>();
			invocationMock->append(throwMock);
			return *this;
		};

		NextProcedureWhenClouse<arglist...>& Return() {
			VoidMock* voidMock = new VoidMock();
			invocationMock->clear();
			invocationMock->append(voidMock);
			return *this;
		};

		NextProcedureWhenClouse<arglist...>& Throw() {
			ThrowMock<void>* throwMock = new ThrowMock<void>();
			invocationMock->clear();
			invocationMock->append(throwMock);
			return *this;
		};

private:
	InvocationMockBase<void, arglist...>* invocationMock;
};

template <typename... arglist>
struct StubProcedureeClouseImpl : public StubProcedureClouse<arglist...> {
	StubProcedureeClouseImpl(MethodMock<void, arglist...>* methodMock) : methodMock(methodMock) {
	}

	FirstProcedureWhenClouse<arglist...>& When(arglist... args) override {
		InvocationMock<void, arglist...> * invocationMock =
			new InvocationMock<void, arglist...>(args...);
		ProcedureWhenClouseImpl<arglist...> * whenClouse =
			new ProcedureWhenClouseImpl<arglist...>(invocationMock);
		methodMock->clear();
		methodMock->append(invocationMock);
		return *whenClouse;
	};

	NextProcedureWhenClouse<arglist...>& Return() override {
		ProcedureWhenClouseImpl<arglist...> * whenClouse = new ProcedureWhenClouseImpl<arglist...>(
			methodMock->first());
		whenClouse->Return();
		return *whenClouse;
	};

	NextProcedureWhenClouse<arglist...>& Throw() override {
		ProcedureWhenClouseImpl<arglist...> * whenClouse = new ProcedureWhenClouseImpl<arglist...>(
			methodMock->first());
		whenClouse->Throw();
		return *whenClouse;
	}

private:
	MethodMock<void, arglist...>* methodMock;
};
#endif // ClousesImpl_h__



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
		invocationMock->append(new DefaultReturnMock<R,arglist...>());
	}

	~FunctionWhenClouseImpl(){}

	NextFunctionWhenClouse<R, arglist...>& ThenReturn(R r) override {
		return ThenDo(std::function<R(arglist...)>([r](...)->R{return r; }));
	};

	NextFunctionWhenClouse<R, arglist...>& ThenThrow() override {
		ThrowMock<R, arglist...>* throwMock = new ThrowMock<R,arglist...>();
		invocationMock->append(throwMock);
		return *this;
	};

	NextFunctionWhenClouse<R, arglist...>& ThenDo(R(*method)(arglist...)) override {
		return ThenDo(std::function<R(arglist...)>(method));
	}

	NextFunctionWhenClouse<R, arglist...>& ThenDo(std::function<R(arglist...)> method) override {
		auto doMock = new DoMock<R, arglist...>(method);
		invocationMock->append(doMock);
		return *this;
	}

	NextFunctionWhenClouse<R, arglist...>& Return(R r) override {
		return Do(std::function<R(arglist...)>([r](...)->R{return r; }));
	};

	NextFunctionWhenClouse<R, arglist...>& Throw() override {
		ThrowMock<R, arglist...>* throwMock = new ThrowMock<R, arglist...>();
		invocationMock->clear();
		invocationMock->append(throwMock);
		return *this;
	};

	NextFunctionWhenClouse<R, arglist...>& Do(R(*method)(arglist...)) override {
		return Do(std::function<R(arglist...)>(method));
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

	FirstFunctionWhenClouse<R, arglist...>& When(arglist... args) override {
		InvocationMockBase<R, arglist...> * invocationMock = methodMock->getInvocationMock(args...);
		if (invocationMock == nullptr) {
			invocationMock = new InvocationMock<R, arglist...>(args...);
			methodMock->append(invocationMock);
		}

		FunctionWhenClouseImpl<R, arglist...> * whenClouse = new FunctionWhenClouseImpl<R, arglist...>
			(invocationMock);
		return *whenClouse;
	}

	NextFunctionWhenClouse<R, arglist...>& Return(R r)  override  {
		return Do(std::function<R(arglist...)>([r](...)->R{return r; }));
	}

	NextFunctionWhenClouse<R, arglist...>& Throw()  override {
		FunctionWhenClouseImpl<R, arglist...> * whenClouse = new FunctionWhenClouseImpl<R, arglist...>(
			methodMock->last());
		whenClouse->Throw();
		return *whenClouse;
	}

	NextFunctionWhenClouse<R, arglist...>& Do(R(*method)(arglist...)) override {
		return Do(std::function<R(arglist...)>(method));
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
			invocationMock->append(new VoidMock<arglist...>());
		}

		virtual ~ProcedureWhenClouseImpl() {}

		NextProcedureWhenClouse<arglist...>& ThenReturn() {
			return ThenDo(std::function<void(arglist...)>([](...)->void{}));
		};

		NextProcedureWhenClouse<arglist...>& ThenThrow() {
			ThrowMock<void, arglist...>* throwMock = new ThrowMock<void, arglist...>();
			invocationMock->append(throwMock);
			return *this;
		};

		NextProcedureWhenClouse<arglist...>& ThenDo(void(*method)(arglist...)) override {
			return ThenDo(std::function<void(arglist...)>(method));
		}

		NextProcedureWhenClouse<arglist...>& ThenDo(std::function<void(arglist...)> method) override {
			auto doMock = new DoMock<void, arglist...>(method);
			invocationMock->append(doMock);
			return *this;
		}

		NextProcedureWhenClouse<arglist...>& Return() {
			return Do(std::function<void(arglist...)>([](...)->void{}));
		};

		NextProcedureWhenClouse<arglist...>& Throw() {
			ThrowMock<void, arglist...>* throwMock = new ThrowMock<void, arglist...>();
			invocationMock->clear();
			invocationMock->append(throwMock);
			return *this;
		};

		NextProcedureWhenClouse<arglist...>& Do(void(*method)(arglist...)) override {
			return Do(std::function<void(arglist...)>(method));
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

	FirstProcedureWhenClouse<arglist...>& When(arglist... args) override {

		InvocationMockBase<void, arglist...> * invocationMock = methodMock->getInvocationMock(args...);
		if (invocationMock == nullptr) {
			invocationMock = new InvocationMock<void, arglist...>(args...);
			methodMock->append(invocationMock);
		}
		ProcedureWhenClouseImpl<arglist...> * whenClouse = new ProcedureWhenClouseImpl<arglist...>(invocationMock);
		return *whenClouse;
	};

	NextProcedureWhenClouse<arglist...>& Return() override {
		ProcedureWhenClouseImpl<arglist...> * whenClouse = new ProcedureWhenClouseImpl<arglist...>(
			methodMock->last());
		whenClouse->Return();
		return *whenClouse;
	};

	NextProcedureWhenClouse<arglist...>& Throw() override {
		ProcedureWhenClouseImpl<arglist...> * whenClouse = new ProcedureWhenClouseImpl<arglist...>(
			methodMock->last());
		whenClouse->Throw();
		return *whenClouse;
	}

	NextProcedureWhenClouse<arglist...>& Do(void(*method)(arglist...)) override {
		return Do(std::function<void(arglist...)>(method));
	}

	NextProcedureWhenClouse<arglist...>& Do(std::function<void(arglist...)> method) override {
		ProcedureWhenClouseImpl<arglist...> * whenClouse = new ProcedureWhenClouseImpl<arglist...>(methodMock->last());
		whenClouse->Do(method);
		return *whenClouse;
	}
	
private:
	MethodMock<void, arglist...>* methodMock;
};
#endif // ClousesImpl_h__



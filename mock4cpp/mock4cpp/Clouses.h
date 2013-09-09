#ifndef Clouses_h__
#define Clouses_h__

#include <type_traits>
#include <functional>

template <typename R, typename... arglist>
struct NextFunctionWhenClouse {
	
	virtual ~NextFunctionWhenClouse() = 0 {};

	NextFunctionWhenClouse<R, arglist...>& ThenReturn(R r)  {
		return ThenDo(std::function<R(arglist...)>([r](...)->R{return r; }));
	}

	template <typename E>
	NextFunctionWhenClouse<R, arglist...>& ThenThrow(E e) {
		return ThenDo(std::function<R(arglist...)>([e](...)->R{throw e; }));
	}

	NextFunctionWhenClouse<R, arglist...>& ThenDo(R(*method)(arglist...)) {
		return ThenDo(std::function<R(arglist...)>(method));
	}

	virtual NextFunctionWhenClouse<R, arglist...>& ThenDo(std::function<R(arglist...)> method) = 0;

};


template <typename R, typename... arglist>
struct FirstFunctionWhenClouse {
	virtual ~FirstFunctionWhenClouse() = 0 {};

	NextFunctionWhenClouse<R, arglist...>& Return(R r)   {
		return Do(std::function<R(arglist...)>([r](...)->R{return r; }));
	}

	template <typename E>
	NextFunctionWhenClouse<R, arglist...>& Throw(E e)  {
		return Do(std::function<R(arglist...)>([e](...)->R{throw e; }));
	}

	NextFunctionWhenClouse<R, arglist...>& Do(R(*method)(arglist...)) {
		return Do(std::function<R(arglist...)>(method));
	}

	virtual NextFunctionWhenClouse<R, arglist...>& Do(std::function<R(arglist...)> method) = 0;
	
};


template <typename R, typename... arglist>
struct StubFunctionClouse : public FirstFunctionWhenClouse<R, arglist...>{

	virtual ~StubFunctionClouse() = 0 {};

	virtual FirstFunctionWhenClouse<R, arglist...>& When(const arglist&...) = 0;
};

template <typename... arglist>
struct NextProcedureWhenClouse {
	virtual ~NextProcedureWhenClouse() = 0 {};

	NextProcedureWhenClouse<arglist...>& ThenReturn() {
		return ThenDo(std::function<void(arglist...)>([](...)->void{}));
	}

	template <typename E>
	NextProcedureWhenClouse<arglist...>& ThenThrow(E e) {
		return ThenDo(std::function<void(arglist...)>([e](...)->void{ throw e; }));
	}

	NextProcedureWhenClouse<arglist...>& ThenDo(void(*method)(arglist...))  {
		return ThenDo(std::function<void(arglist...)>(method));
	}

	virtual NextProcedureWhenClouse<arglist...>& ThenDo(std::function<void(arglist...)> method) = 0;

};


template <typename... arglist>
struct FirstProcedureWhenClouse {

	virtual ~FirstProcedureWhenClouse() = 0 {};

	NextProcedureWhenClouse<arglist...>& Return() {
		return Do(std::function<void(arglist...)>([](...)->void{}));
	};

	template <typename E>
	NextProcedureWhenClouse<arglist...>& Throw(E e) {
		return Do(std::function<void(arglist...)>([e](...)->void{ throw e; }));
	};

	NextProcedureWhenClouse<arglist...>& Do(void(*method)(arglist...)) {
		return Do(std::function<void(arglist...)>(method));
	}

	virtual NextProcedureWhenClouse<arglist...>& Do(std::function<void(arglist...)> method) = 0;
};

template <typename... arglist>
struct StubProcedureClouse : public FirstProcedureWhenClouse<arglist...>{

	virtual ~StubProcedureClouse() = 0 {};

	virtual FirstProcedureWhenClouse<arglist...>& When(const arglist&...) = 0;
};

#endif // Clouses_h__

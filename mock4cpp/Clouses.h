#ifndef Clouses_h__
#define Clouses_h__

template <typename R, typename... arglist>
struct NextFunctionWhenClouse {
	
	virtual ~NextFunctionWhenClouse() = 0 {};

	virtual NextFunctionWhenClouse<R, arglist...>& ThenReturn(R r) = 0;

	virtual NextFunctionWhenClouse<R, arglist...>& ThenThrow() = 0;
};


template <typename R, typename... arglist>
struct FirstFunctionWhenClouse {
	virtual ~FirstFunctionWhenClouse() = 0 {};

	virtual NextFunctionWhenClouse<R, arglist...>& Return(R r) = 0;

	virtual NextFunctionWhenClouse<R, arglist...>& Throw() = 0;

	virtual NextFunctionWhenClouse<R, arglist...>& Do(R(*method)(arglist...)) = 0;

};


template <typename R, typename... arglist>
struct StubFunctionClouse : public FirstFunctionWhenClouse<R, arglist...>{

	virtual ~StubFunctionClouse() = 0 {};

	virtual FirstFunctionWhenClouse<R, arglist...>& When(arglist...) = 0;
};

template <typename... arglist>
struct NextProcedureWhenClouse {
	virtual ~NextProcedureWhenClouse() = 0 {};

	virtual NextProcedureWhenClouse<arglist...>& ThenReturn() = 0;

	virtual NextProcedureWhenClouse<arglist...>& ThenThrow() = 0;
};


template <typename... arglist>
struct FirstProcedureWhenClouse {

	virtual ~FirstProcedureWhenClouse() = 0 {};

	virtual NextProcedureWhenClouse<arglist...>& Return() = 0;

	virtual NextProcedureWhenClouse<arglist...>& Throw() = 0;

	virtual NextProcedureWhenClouse<arglist...>& Do(void(*method)(arglist...)) = 0;

};

template <typename... arglist>
struct StubProcedureClouse : public FirstProcedureWhenClouse<arglist...>{

	virtual ~StubProcedureClouse() = 0 {};

	virtual FirstProcedureWhenClouse<arglist...>& When(arglist...) = 0;
};

#endif // Clouses_h__

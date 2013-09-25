#ifndef Clouses_h__
#define Clouses_h__

#include <functional>
#include <type_traits>
#include "../mockutils/traits.h"
#include "../mockutils/DefaultValue.hpp"

template <typename R, typename... arglist>
struct NextFunctionWhenClouse {
	
	virtual ~NextFunctionWhenClouse() {};

	template<typename NO_REF = std::remove_reference<R>::type>
	typename std::enable_if<is_copy_initializable<NO_REF>::value, NextFunctionWhenClouse<R, arglist...>&>::type
		ThenReturn(const R& r) {
			return ThenDo(std::function<R(arglist...)>([r](...)->R{
				return r;
			}));
		}

	template<typename NO_REF = std::remove_reference<R>::type>
	typename std::enable_if<!is_copy_initializable<NO_REF>::value, NextFunctionWhenClouse<R, arglist...>&>::type
		ThenReturn(const R& r) {
			return ThenDo(std::function<R(arglist...)>([&r](...)->R{
				return r;
			}));
		}

	template <typename E>
	NextFunctionWhenClouse<R, arglist...>& ThenThrow(const E& e) {
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
	
 	template<typename NO_REF = std::remove_reference<R>::type>
	typename std::enable_if<std::is_trivially_copy_constructible<NO_REF>::value, NextFunctionWhenClouse<R, arglist...>&>::type
		Return(const R& r) {
			return Do(std::function<R(arglist...)>([r](...)->R{
				return r;
			}));
		}

	template<typename NO_REF = std::remove_reference<R>::type>
	typename std::enable_if<!std::is_trivially_copy_constructible<NO_REF>::value, NextFunctionWhenClouse<R, arglist...>&>::type
		Return(const R& r) {
			return Do(std::function<R(arglist...)>([&r](...)->R{
				return r;
			}));
		}

	template <typename E>
	NextFunctionWhenClouse<R, arglist...>& Throw(const E& e)  {
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
	NextProcedureWhenClouse<arglist...>& ThenThrow(const E e) {
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
	NextProcedureWhenClouse<arglist...>& Throw(const E e) {
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


template <typename... arglist>
struct StubDataMemberClouse {

	virtual ~StubDataMemberClouse() = 0 {};


	virtual void operator()(std::initializer_list<arglist...> list) = 0;
};


template <typename MEMBER_TYPE>
struct StubDataMemberClouse2 {

	typename std::enable_if<std::is_default_constructible<MEMBER_TYPE>::value, void>::type 
		Init() 
	{
	}

	typename std::enable_if<std::is_copy_constructible<MEMBER_TYPE>::value, void>::type 
		Init(const MEMBER_TYPE& other)
	{
	}

	template <typename... arglist>
	typename std::enable_if<std::is_constructible<MEMBER_TYPE>::value, void>::type
		Init(const arglist&... params)
	{
	}
};

#endif // Clouses_h__
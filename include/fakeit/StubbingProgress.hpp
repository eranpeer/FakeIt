/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef stubbing_h__
#define stubbing_h__

#include <functional>
#include <type_traits>
#include <stdexcept>
#include <utility>

#include "fakeit/FakeitExceptions.hpp"
#include "fakeit/ActualInvocation.hpp"
#include "fakeit/Quantifier.hpp"
#include "fakeit/Action.hpp"
#include "mockutils/DefaultValue.hpp"
#include "mockutils/Macros.hpp"

namespace fakeit {

template<typename R, typename ... arglist>
struct FunctionStubbingProgress {

	virtual ~FunctionStubbingProgress() THROWS {
	}

	template<typename U = R>
	typename std::enable_if<!std::is_reference<U>::value, FunctionStubbingProgress<R, arglist...>&>::type 
	Return(const R& r) {
		return Do([r](const arglist&...)->R {return r;});
	}

	template<typename U = R>
	typename std::enable_if<std::is_reference<U>::value, FunctionStubbingProgress<R, arglist...>&>::type 
	Return(const R& r) {
		return Do([&r](const arglist&...)->R {return r;});
	}

	FunctionStubbingProgress<R, arglist...>&
	Return(const Quantifier<R>& q) {
		const R& value = q.value;
		auto method = [value](const arglist&...)->R {return value;};
		return DoImpl(new Repeat<R, arglist...>(method, q.quantity));
	}

	template<typename first, typename second, typename ... tail>
	FunctionStubbingProgress<R, arglist...>&
	Return(const first& f, const second& s, const tail&... t) {
		Return(f);
		return Return(s, t...);
	}


	template<typename U = R>
	typename std::enable_if<!std::is_reference<U>::value, void>::type
	AlwaysReturn(const R& r) {
		return AlwaysDo([r](const arglist&...)->R {return r;});
	}

	template<typename U = R>
	typename std::enable_if<std::is_reference<U>::value,void>::type
	AlwaysReturn(const R& r) {
		return AlwaysDo([&r](const arglist&...)->R {return r;});
	}

	FunctionStubbingProgress<R, arglist...>&
	Return() {
		return Do([](const arglist&...)->R {return DefaultValue<R>::value();});
	}

	void AlwaysReturn() {
		return AlwaysDo([](const arglist&...)->R {return DefaultValue<R>::value();});
	}

	template<typename E>
	FunctionStubbingProgress<R, arglist...>& Throw(const E& e) {
		return Do([e](const arglist&...)->R {throw e;});
	}

	template<typename E>
	FunctionStubbingProgress<R, arglist...>&
	Throw(const Quantifier<E>& q) {
		const E& value = q.value;
		auto method = [value](const arglist&...)->R {throw value;};
		return DoImpl(new Repeat<R, arglist...>(method, q.quantity));
	}

	template<typename first, typename second, typename ... tail>
	FunctionStubbingProgress<R, arglist...>&
	Throw(const first& f, const second& s, const tail&... t) {
		Throw(f);
		return Throw(s, t...);
	}

	template<typename E>
	void AlwaysThrow(const E& e) {
		return AlwaysDo([e](const arglist&...)->R {throw e;});
	}

	virtual FunctionStubbingProgress<R, arglist...>&
	Do(std::function<R(arglist...)> method) {
		return DoImpl(new Repeat<R, arglist...>(method));
	}

	template<typename F>
	FunctionStubbingProgress<R, arglist...>&
	Do(const Quantifier<F>& q) {
		return DoImpl(new Repeat<R, arglist...>(q.value, q.quantity));
	}

	template<typename first, typename second, typename ... tail>
	FunctionStubbingProgress<R, arglist...>&
	Do(const first& f, const second& s, const tail&... t) {
		Do(f);
		return Do(s, t...);
	}

	virtual void AlwaysDo(std::function<R(arglist...)> method) {
		DoImpl(new RepeatForever<R, arglist...>(method));
	}

protected:

	virtual FunctionStubbingProgress<R, arglist...>& DoImpl(Action<R, arglist...>* action) = 0;

private:
	FunctionStubbingProgress & operator=(const FunctionStubbingProgress & other) = delete;
};

template<typename R, typename ... arglist>
struct ProcedureStubbingProgress {

	virtual ~ProcedureStubbingProgress() THROWS {
	}

	ProcedureStubbingProgress<R, arglist...>& Return() {
		return Do([](const arglist&...)->R {return DefaultValue<R>::value();});
	}

	void AlwaysReturn() {
		return AlwaysDo([](const arglist&...)->R {return DefaultValue<R>::value();});
	}

	ProcedureStubbingProgress<R, arglist...>&
	Return(const Quantifier<R>& q) {
		auto method = [](const arglist&...)->R {return DefaultValue<R>::value();};
		return DoImpl(new Repeat<R, arglist...>(method, q.quantity));
	}

	template<typename E>
	ProcedureStubbingProgress<R, arglist...>& Throw(const E& e) {
		return Do([e](const arglist&...)->R {throw e;});
	}

	template<typename E>
	ProcedureStubbingProgress<R, arglist...>&
	Throw(const Quantifier<E>& q) {
		const E& value = q.value;
		auto method = [value](const arglist&...)->R {throw value;};
		return DoImpl(new Repeat<R, arglist...>(method, q.quantity));
	}

	template<typename first, typename second, typename ... tail>
	ProcedureStubbingProgress<R, arglist...>&
	Throw(const first& f, const second& s, const tail&... t) {
		Throw(f);
		return Throw(s, t...);
	}

	template<typename E>
	void AlwaysThrow(const E e) {
		return AlwaysDo([e](const arglist&...)->R {throw e;});
	}

	virtual ProcedureStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) {
		return DoImpl(new Repeat<R, arglist...>(method));
	}

	template<typename F>
	ProcedureStubbingProgress<R, arglist...>&
	Do(const Quantifier<F>& q) {
		return DoImpl(new Repeat<R, arglist...>(q.value, q.quantity));
	}

	template<typename first, typename second, typename ... tail>
	ProcedureStubbingProgress<R, arglist...>&
	Do(const first& f, const second& s, const tail&... t) {
		Do(f);
		return Do(s, t...);
	}

	virtual void AlwaysDo(std::function<R(arglist...)> method) {
		DoImpl(new RepeatForever<R, arglist...>(method));
	}

protected:

	virtual ProcedureStubbingProgress<R, arglist...>& DoImpl(Action<R, arglist...>* action)=0;

private:
	ProcedureStubbingProgress & operator=(const ProcedureStubbingProgress & other) = delete;
};

}

#endif // stubbing_h__

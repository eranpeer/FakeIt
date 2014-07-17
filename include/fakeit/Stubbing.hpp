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
#include <unordered_set>
#include <vector>
#include <stdexcept>
#include <utility>

#include "fakeit/FakeitExceptions.hpp"
#include "fakeit/ActualInvocation.hpp"
#include "fakeit/quantifier.hpp"
#include "mockutils/DefaultValue.hpp"
#include "mockutils/Macros.hpp"

namespace fakeit {

struct MethodVerificationProgress {

	MethodVerificationProgress() {
	}

	virtual ~MethodVerificationProgress() THROWS {
	}

	void Never() {
		Exactly(0);
	}

	virtual void Once() {
		Exactly(1);
	}

	virtual void Twice() {
		Exactly(2);
	}

	virtual void AtLeastOnce() {
		verifyInvocations(-1);
	}

	virtual void Exactly(const int times) {
		if (times < 0) {
			throw std::invalid_argument(std::string("bad argument times:").append(std::to_string(times)));
		}
		verifyInvocations(times);
	}

	virtual void Exactly(const Quantity & q) {
		Exactly(q.quantity);
	}

	virtual void AtLeast(const int times) {
		if (times < 0) {
			throw std::invalid_argument(std::string("bad argument times:").append(std::to_string(times)));
		}
		verifyInvocations(-times);
	}

	virtual void AtLeast(const Quantity & q) {
		AtLeast(q.quantity);
	}

	virtual MethodVerificationProgress& setFileInfo(std::string file, int line, std::string testMethod) = 0;

protected:
	virtual void verifyInvocations(const int times) = 0;
private:
	MethodVerificationProgress & operator=(const MethodVerificationProgress & other) = delete;
};


template<typename R, typename ... arglist>
struct FirstFunctionStubbingProgress {

	virtual ~FirstFunctionStubbingProgress() THROWS {
	}

	template<typename U = R>
	typename std::enable_if<!std::is_reference<U>::value, FirstFunctionStubbingProgress<R, arglist...>&>::type 
	Return(const R& r) {
		return Do([r](const arglist&...)->R {return r;});
	}

	template<typename U = R>
	typename std::enable_if<std::is_reference<U>::value, FirstFunctionStubbingProgress<R, arglist...>&>::type 
	Return(const R& r) {
		return Do([&r](const arglist&...)->R {return r;});
	}

	FirstFunctionStubbingProgress<R, arglist...>&
	Return(const Quantifier<R>& q) {
		const R& value = q.value;
		auto method = [value](const arglist&...)->R {return value;};
		std::shared_ptr<Behavior<R, arglist...>> doMock { new Repeat<R, arglist...>(method, q.quantity) };
		return DoImpl(doMock);
	}

	template<typename first, typename second, typename ... tail>
	FirstFunctionStubbingProgress<R, arglist...>&
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

	FirstFunctionStubbingProgress<R, arglist...>&
	Return() {
		return Do([](const arglist&...)->R {return DefaultValue::value<R>();});
	}

	void AlwaysReturn() {
		return AlwaysDo([](const arglist&...)->R {return DefaultValue::value<R>();});
	}

	template<typename E>
	FirstFunctionStubbingProgress<R, arglist...>& Throw(const E& e) {
		return Do([e](const arglist&...)->R {throw e;});
	}

	template<typename E>
	FirstFunctionStubbingProgress<R, arglist...>&
	Throw(const Quantifier<E>& q) {
		const E& value = q.value;
		auto method = [value](const arglist&...)->R {throw value;};
		std::shared_ptr<Behavior<R, arglist...>> doMock { new Repeat<R, arglist...>(method, q.quantity) };
		return DoImpl(doMock);
	}

	template<typename first, typename second, typename ... tail>
	FirstFunctionStubbingProgress<R, arglist...>&
	Throw(const first& f, const second& s, const tail&... t) {
		Throw(f);
		return Throw(s, t...);
	}

	template<typename E>
	void AlwaysThrow(const E& e) {
		return AlwaysDo([e](const arglist&...)->R {throw e;});
	}

	virtual FirstFunctionStubbingProgress<R, arglist...>&
	Do(std::function<R(arglist...)> method) {
		std::shared_ptr<Behavior<R, arglist...>> ptr { new Repeat<R, arglist...>(method) };
		return DoImpl(ptr);
	}

	template<typename F>
	FirstFunctionStubbingProgress<R, arglist...>&
	Do(const Quantifier<F>& q) {
		std::shared_ptr<Behavior<R, arglist...>> doMock { new Repeat<R, arglist...>(q.value, q.quantity) };
		return DoImpl(doMock);
	}

	template<typename first, typename second, typename ... tail>
	FirstFunctionStubbingProgress<R, arglist...>&
	Do(const first& f, const second& s, const tail&... t) {
		Do(f);
		return Do(s, t...);
	}

	virtual void AlwaysDo(std::function<R(arglist...)> method) {
		std::shared_ptr<Behavior<R, arglist...>> ptr { new RepeatForever<R, arglist...>(method) };
		DoImpl(ptr);
	}

protected:

	virtual FirstFunctionStubbingProgress<R, arglist...>& DoImpl(std::shared_ptr<Behavior<R, arglist...> > ptr) = 0;

private:
	FirstFunctionStubbingProgress & operator=(const FirstFunctionStubbingProgress & other) = delete;
};

template<typename R, typename ... arglist>
struct FirstProcedureStubbingProgress {

	virtual ~FirstProcedureStubbingProgress() THROWS {
	}

	FirstProcedureStubbingProgress<R, arglist...>& Return() {
		return Do([](const arglist&...)->R {return DefaultValue::value<R>();});
	}

	void AlwaysReturn() {
		return AlwaysDo([](const arglist&...)->R {return DefaultValue::value<R>();});
	}

	FirstProcedureStubbingProgress<R, arglist...>&
	Return(const Quantifier<R>& q) {
		auto method = [](const arglist&...)->R {return DefaultValue::value<R>();};
		std::shared_ptr<Behavior<R, arglist...>> doMock { new Repeat<R, arglist...>(method, q.quantity) };
		return DoImpl(doMock);
	}

	template<typename E>
	FirstProcedureStubbingProgress<R, arglist...>& Throw(const E& e) {
		return Do([e](const arglist&...)->R {throw e;});
	}

	template<typename E>
	FirstProcedureStubbingProgress<R, arglist...>&
	Throw(const Quantifier<E>& q) {
		const E& value = q.value;
		auto method = [value](const arglist&...)->R {throw value;};
		std::shared_ptr<Behavior<R, arglist...>> doMock { new Repeat<R, arglist...>(method, q.quantity) };
		return DoImpl(doMock);
	}

	template<typename first, typename second, typename ... tail>
	FirstProcedureStubbingProgress<R, arglist...>&
	Throw(const first& f, const second& s, const tail&... t) {
		Throw(f);
		return Throw(s, t...);
	}

	template<typename E>
	void AlwaysThrow(const E e) {
		return AlwaysDo([e](const arglist&...)->R {throw e;});
	}

	virtual FirstProcedureStubbingProgress<R, arglist...>& Do(std::function<R(arglist...)> method) {
		std::shared_ptr<Behavior<R, arglist...>> ptr { new Repeat<R, arglist...>(method) };
		return DoImpl(ptr);
	}

	template<typename F>
	FirstProcedureStubbingProgress<R, arglist...>&
	Do(const Quantifier<F>& q) {
		std::shared_ptr<Behavior<R, arglist...>> doMock { new Repeat<R, arglist...>(q.value, q.quantity) };
		return DoImpl(doMock);
	}

	template<typename first, typename second, typename ... tail>
	FirstProcedureStubbingProgress<R, arglist...>&
	Do(const first& f, const second& s, const tail&... t) {
		Do(f);
		return Do(s, t...);
	}

	virtual void AlwaysDo(std::function<R(arglist...)> method) {
		std::shared_ptr<Behavior<R, arglist...>> ptr { new RepeatForever<R, arglist...>(method) };
		DoImpl(ptr);
	}

protected:

	virtual FirstProcedureStubbingProgress<R, arglist...>& DoImpl(std::shared_ptr<Behavior<R, arglist...> > ptr)=0;

private:
	FirstProcedureStubbingProgress & operator=(const FirstProcedureStubbingProgress & other) = delete;
};

template<typename R, typename ... arglist>
struct FunctionStubbingProgress: public virtual FirstFunctionStubbingProgress<R, arglist...> {

	FunctionStubbingProgress() = default;
	virtual ~FunctionStubbingProgress() {
	}

private:
	FunctionStubbingProgress & operator=(const FunctionStubbingProgress & other) = delete;
};

template<typename R, typename ... arglist>
struct ProcedureStubbingProgress: //
public virtual FirstProcedureStubbingProgress<R, arglist...> {

	ProcedureStubbingProgress() = default;
	virtual ~ProcedureStubbingProgress() override {
	}

private:
	ProcedureStubbingProgress & operator=(const ProcedureStubbingProgress & other) = delete;
};

}

#endif // stubbing_h__

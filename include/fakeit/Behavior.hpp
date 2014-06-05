/*
 * Behavior.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Jun 5, 2014
 */
#ifndef BEHAVIOR_HPP_
#define BEHAVIOR_HPP_

#include <vector>
#include <functional>
#include <atomic>
#include <tuple>

#include "mockutils/DefaultValue.hpp"
#include "fakeit/FakeitExceptions.hpp"

namespace fakeit {

template<typename R, typename ... arglist>
struct Behavior {
	virtual R invoke(arglist&... args) = 0;
	virtual bool isDone() = 0;
};

template<typename R, typename ... arglist>
struct Call: public Behavior<R, arglist...> {
	Call(std::function<R(arglist&...)> f) :
			f(f), times(1) {
	}

	Call(std::function<R(arglist&...)> f, long times) :
			f(f), times(times) {
	}

	virtual R invoke(arglist&... args) override {
		times--;
		return f(args...);
	}

	virtual bool isDone() override {
		return times == 0;
	}
private:
	std::function<R(arglist&...)> f;
	long times;
};

template<typename R, typename ... arglist>
struct CallForever: public Behavior<R, arglist...> {
	CallForever(std::function<R(arglist&...)> f) :
			f(f) {
	}
	virtual R invoke(arglist&... args) override {
		return f(args...);
	}

	virtual bool isDone() override {
		return false;
	}
private:
	std::function<R(arglist&...)> f;
};

template<typename R, typename ... arglist>
struct ThrowUnexpectedMethodCall: public Behavior<R, arglist...> {
	virtual R invoke(arglist&... args) override {
		throw UnexpectedMethodCallException();
	}

	virtual bool isDone() override {
		return false;
	}
};

template<typename R, typename ... arglist>
struct ReturnDefaultValue: public Behavior<R, arglist...> {
	virtual R invoke(arglist&... args) override {
		return DefaultValue::value<R>();
	}

	virtual bool isDone() override {
		return false;
	}
};

template<typename C, typename R, typename ... arglist>
struct ReturnDelegateValue: public Behavior<R, arglist...> {

	ReturnDelegateValue(C & instance, R (C::*vMethod)(arglist... args)):instance(instance),vMethod(vMethod){}

	virtual R invoke(arglist&... args) override {
		return ((&instance)->*vMethod)(args...);
	}

	virtual bool isDone() override {
		return false;
	}

private:
	C & instance;
	R (C::*vMethod)(arglist... args);
};

}

#endif /* BEHAVIOR_HPP_ */

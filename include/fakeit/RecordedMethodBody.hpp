/*
 * RecordedMethodBody.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Aug 30, 2014
 */
#ifndef RECORDEDMETHODBODY_HPP_
#define RECORDEDMETHODBODY_HPP_

#include <vector>
#include <functional>
#include <tuple>

#include "mockutils/TupleDispatcher.hpp"
#include "fakeit/DomainObjects.hpp"
#include "fakeit/ActualInvocation.hpp"
#include "fakeit/Behavior.hpp"
#include "fakeit/matchers.hpp"
#include "fakeit/FakeitContext.hpp"

namespace fakeit {

class finally {
private:
	std::function<void()> finallyClause;
	finally(const finally &);
	finally& operator=(const finally &);
public:
	explicit finally(std::function<void()> f) :
			finallyClause(f) {
	}

	~finally() {
		finallyClause();
	}
};

struct NoMoreRecordedBehaviorException: public std::exception {
};

template<typename R, typename ... arglist>
struct RecordedMethodBody: public MethodInvocationHandler<R, arglist...> {

	RecordedMethodBody(Method & method) :
			method(method) {
		clear();
	}

	void AppendDo(std::function<R(arglist...)> method) {
		std::shared_ptr<Behavior<R, arglist...>> doMock = std::shared_ptr<Behavior<R, arglist...>> { new Repeat<R, arglist...>(method) };
		AppendDo(doMock);
	}

	void LastDo(std::function<R(arglist...)> method) {
		std::shared_ptr<Behavior<R, arglist...>> doMock = std::shared_ptr<Behavior<R, arglist...>> { new Repeat<R, arglist...>(method) };
		LastDo(doMock);
	}

	void AppendDo(std::shared_ptr<Behavior<R, arglist...> > doMock) {
		append(doMock);
	}

	void LastDo(std::shared_ptr<Behavior<R, arglist...> > doMock) {
		append(doMock);
		behaviorMocks.pop_back();
	}

	R handleMethodInvocation(arglist&... args) override {
		std::shared_ptr<Behavior<R, arglist...>> behavior = behaviorMocks.front();
		std::function < void() > finallyClause = [&]()->void {
			if (behavior->isDone())
			behaviorMocks.erase(behaviorMocks.begin());
		};
		finally onExit(finallyClause);
		return behavior->invoke(args...);
	}

private:

	struct NoMoreRecordedBehavior: public Behavior<R, arglist...> {

		virtual ~NoMoreRecordedBehavior() = default;

		virtual R invoke(arglist&...) override {
			throw NoMoreRecordedBehaviorException();
		}

		virtual bool isDone() override {
			return false;
		}
	};

	Method & method;

	void append(std::shared_ptr<Behavior<R, arglist...>> mock) {
		behaviorMocks.insert(behaviorMocks.end() - 1, mock);
	}

	void clear() {
		behaviorMocks.clear();
		auto doMock = std::shared_ptr<Behavior<R, arglist...>> { new NoMoreRecordedBehavior() };
		behaviorMocks.push_back(doMock);
	}

	std::vector<std::shared_ptr<Behavior<R, arglist...>>>behaviorMocks;
};

}
#endif /* RECORDEDMETHODBODY_HPP_ */

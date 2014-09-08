/*
 * ActionSequence.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Aug 30, 2014
 */
#ifndef ActionSequence_hpp_
#define ActionSequence_hpp_

#include <vector>

#include "fakeit/DomainObjects.hpp"
#include "fakeit/ActualInvocation.hpp"
#include "fakeit/Action.hpp"
#include "fakeit/matchers.hpp"

#include "mockutils/Finally.hpp"
#include "mockutils/MethodInvocationHandler.hpp"

namespace fakeit {

/**
 * Represents a list of recorded actions created by one stubbing line:
 * When(Method(mock,foo)).Return(1).Throw(e).AlwaysReturn(2);
 *                        ^--------Action-Sequence---------^
 */
template<typename R, typename ... arglist>
struct ActionSequence: public MethodInvocationHandler<R, arglist...> {

	ActionSequence() {
		clear();
	}

	void AppendDo(std::function<R(arglist...)> method) {
		Action<R, arglist...> action = new Repeat<R, arglist...>(method);
		AppendDo(action);
	}

	void LastDo(std::function<R(arglist...)> method) {
		Action<R, arglist...> action = new Repeat<R, arglist...>(method);
		LastDo(action);
	}

	void AppendDo(Action<R, arglist...>* action) {
		append(action);
	}

	void LastDo(Action<R, arglist...>* action) {
		append(action);
		_recordedActions.pop_back();
	}

	R handleMethodInvocation(arglist&... args) override {
		std::shared_ptr<Destructable> destructablePtr = _recordedActions.front();
		Destructable& destructable = *destructablePtr;
		Action<R, arglist...>& action = dynamic_cast<Action<R, arglist...>&>(destructable);
		std::function < void() > finallyClause = [&]()->void {
			if (action.isDone())
			_recordedActions.erase(_recordedActions.begin());
		};
		Finally onExit(finallyClause);
		return action.invoke(args...);
	}

private:

	struct NoMoreRecordedAction: public Action<R, arglist...> {

		virtual ~NoMoreRecordedAction() = default;

		virtual R invoke(arglist&...) override {
			throw NoMoreRecordedActionException();
		}

		virtual bool isDone() override {
			return false;
		}
	};

	void append(Action<R, arglist...>* action) {
		std::shared_ptr<Destructable> destructable{action};
		_recordedActions.insert(_recordedActions.end() - 1, destructable);
	}

	void clear() {
		_recordedActions.clear();
		auto actionPtr = std::shared_ptr<Destructable> { new NoMoreRecordedAction() };
		_recordedActions.push_back(actionPtr);
	}

	std::vector<std::shared_ptr<Destructable>>_recordedActions;
};

}
#endif

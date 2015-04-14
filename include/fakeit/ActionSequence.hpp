/*
 * ActionSequence.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Aug 30, 2014
 */
#pragma once

#include <vector>

#include "fakeit/DomainObjects.hpp"
#include "fakeit/ActualInvocation.hpp"
#include "fakeit/Action.hpp"
#include "fakeit/invocation_matchers.hpp"

#include "mockutils/Finally.hpp"
#include "mockutils/MethodInvocationHandler.hpp"

namespace fakeit {

/**
 * Represents a list of recorded actions created by one stubbing line:
 * When(Method(mock,foo)).Return(1).Throw(e).AlwaysReturn(2);
 *                        ^--------Action-Sequence---------^
 */
    template<typename R, typename ... arglist>
    struct ActionSequence : public MethodInvocationHandler<R, arglist...> {

        ActionSequence() {
            clear();
        }

        void AppendDo(Action<R, arglist...> *action) {
            append(action);
        }

        R handleMethodInvocation(arglist &... args) override {
            std::shared_ptr<Destructible> destructablePtr = _recordedActions.front();
            Destructible &destructable = *destructablePtr;
            Action<R, arglist...> &action = dynamic_cast<Action<R, arglist...> &>(destructable);
            std::function<void()> finallyClause = [&]() -> void {
                if (action.isDone())
                    _recordedActions.erase(_recordedActions.begin());
            };
            Finally onExit(finallyClause);
            return action.invoke(args...);
        }

    private:

        struct NoMoreRecordedAction : public Action<R, arglist...> {

            virtual ~NoMoreRecordedAction() = default;

            virtual R invoke(arglist &...) override {
                throw NoMoreRecordedActionException();
            }

            virtual bool isDone() override {
                return false;
            }
        };

        void append(Action<R, arglist...> *action) {
            std::shared_ptr<Destructible> destructable{action};
            _recordedActions.insert(_recordedActions.end() - 1, destructable);
        }

        void clear() {
            _recordedActions.clear();
            auto actionPtr = std::shared_ptr<Destructible> {new NoMoreRecordedAction()};
            _recordedActions.push_back(actionPtr);
        }

        std::vector<std::shared_ptr<Destructible>> _recordedActions;
    };

}

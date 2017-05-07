/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include <vector>
#include <functional>
#include <tuple>

#include "mockutils/TupleDispatcher.hpp"
#include "fakeit/DomainObjects.hpp"
#include "fakeit/ActualInvocation.hpp"
#include "fakeit/ActualInvocationHandler.hpp"
#include "fakeit/invocation_matchers.hpp"
#include "fakeit/FakeitEvents.hpp"
#include "fakeit/FakeitExceptions.hpp"
#include "mockutils/MethodInvocationHandler.hpp"

namespace fakeit {

/**
 * A composite MethodInvocationHandler that holds a list of ActionSequence objects.
 */
    template<typename R, typename ... arglist>
    class RecordedMethodBody : public MethodInvocationHandler<R, arglist...>, public ActualInvocationsSource, public ActualInvocationsContainer {

        struct MatchedInvocationHandler : ActualInvocationHandler<R, arglist...> {

            virtual ~MatchedInvocationHandler() = default;

            MatchedInvocationHandler(typename ActualInvocation<arglist...>::Matcher *matcher,
                ActualInvocationHandler<R, arglist...> *invocationHandler) :
                    _matcher{matcher}, _invocationHandler{invocationHandler} {
            }

            virtual R handleMethodInvocation(ArgumentsTuple<arglist...> & args) override
            {
                Destructible &destructable = *_invocationHandler;
                ActualInvocationHandler<R, arglist...> &invocationHandler = dynamic_cast<ActualInvocationHandler<R, arglist...> &>(destructable);
                return invocationHandler.handleMethodInvocation(args);
            }

            typename ActualInvocation<arglist...>::Matcher &getMatcher() const {
                Destructible &destructable = *_matcher;
                typename ActualInvocation<arglist...>::Matcher &matcher = dynamic_cast<typename ActualInvocation<arglist...>::Matcher &>(destructable);
                return matcher;
            }

        private:
            std::shared_ptr<Destructible> _matcher;
            std::shared_ptr<Destructible> _invocationHandler;
        };


        FakeitContext &_fakeit;
        MethodInfo _method;

        std::vector<std::shared_ptr<Destructible>> _invocationHandlers;
        std::vector<std::shared_ptr<Destructible>> _actualInvocations;

        MatchedInvocationHandler *buildMatchedInvocationHandler(
                typename ActualInvocation<arglist...>::Matcher *invocationMatcher,
                ActualInvocationHandler<R, arglist...> *invocationHandler) {
            return new MatchedInvocationHandler(invocationMatcher, invocationHandler);
        }

        MatchedInvocationHandler *getInvocationHandlerForActualArgs(ActualInvocation<arglist...> &invocation) {
            for (auto i = _invocationHandlers.rbegin(); i != _invocationHandlers.rend(); ++i) {
                std::shared_ptr<Destructible> curr = *i;
                Destructible &destructable = *curr;
                MatchedInvocationHandler &im = asMatchedInvocationHandler(destructable);
                if (im.getMatcher().matches(invocation)) {
                    return &im;
                }
            }
            return nullptr;
        }

        MatchedInvocationHandler &asMatchedInvocationHandler(Destructible &destructable) {
            MatchedInvocationHandler &im = dynamic_cast<MatchedInvocationHandler &>(destructable);
            return im;
        }

        ActualInvocation<arglist...> &asActualInvocation(Destructible &destructable) const {
            ActualInvocation<arglist...> &invocation = dynamic_cast<ActualInvocation<arglist...> &>(destructable);
            return invocation;
        }

    public:

        RecordedMethodBody(FakeitContext &fakeit, std::string name) :
                _fakeit(fakeit), _method{MethodInfo::nextMethodOrdinal(), name} { }

        virtual ~RecordedMethodBody() NO_THROWS {
        }

        MethodInfo &getMethod() {
            return _method;
        }

        bool isOfMethod(MethodInfo &method) {
            //return &method == &_method;
            return method.id() == _method.id();
        }

        void addMethodInvocationHandler(typename ActualInvocation<arglist...>::Matcher *matcher,
            ActualInvocationHandler<R, arglist...> *invocationHandler) {
            ActualInvocationHandler<R, arglist...> *mock = buildMatchedInvocationHandler(matcher, invocationHandler);
            std::shared_ptr<Destructible> destructable{mock};
            _invocationHandlers.push_back(destructable);
        }

        void reset() { // this method is never used!! Delete !!!
            _invocationHandlers.clear();
            _actualInvocations.clear();
        }

		void clear() override { // this method is never used!! Delete !!!
			_actualInvocations.clear();
		}

        R handleMethodInvocation(const typename fakeit::production_arg<arglist>::type... args) override {
            unsigned int ordinal = Invocation::nextInvocationOrdinal();
            MethodInfo &method = this->getMethod();
            auto actualInvocation = new ActualInvocation<arglist...>(ordinal, method, std::forward<const typename fakeit::production_arg<arglist>::type>(args)...);

            // ensure deletion if not added to actual invocations.
            std::shared_ptr<Destructible> actualInvocationDtor{actualInvocation};

            auto invocationHandler = getInvocationHandlerForActualArgs(*actualInvocation);
            if (invocationHandler) {
                auto &matcher = invocationHandler->getMatcher();
                actualInvocation->setActualMatcher(&matcher);
                _actualInvocations.push_back(actualInvocationDtor);
                try {
                    return invocationHandler->handleMethodInvocation(actualInvocation->getActualArguments());
                } catch (NoMoreRecordedActionException &) {
                }
            }

            UnexpectedMethodCallEvent event(UnexpectedType::Unmatched, *actualInvocation);
            _fakeit.handle(event);
            std::string format{_fakeit.format(event)};
            UnexpectedMethodCallException e(format);
            throw e;
        }

        void scanActualInvocations(const std::function<void(ActualInvocation<arglist...> &)> &scanner) {
            for (auto destructablePtr : _actualInvocations) {
                ActualInvocation<arglist...> &invocation = asActualInvocation(*destructablePtr);
                scanner(invocation);
            }
        }

        void getActualInvocations(std::unordered_set<Invocation *> &into) const override {
            for (auto destructablePtr : _actualInvocations) {
                Invocation &invocation = asActualInvocation(*destructablePtr);
                into.insert(&invocation);
            }
        }

        void setMethodDetails(const std::string &mockName, const std::string &methodName) {
            const std::string fullName{mockName + "." + methodName};
            _method.setName(fullName);
        }

    };

}

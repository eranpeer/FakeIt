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
#include "fakeit/invocation_matchers.hpp"
#include "fakeit/FakeitEvents.hpp"
#include "fakeit/FakeitExceptions.hpp"
#include "mockutils/MethodInvocationHandler.hpp"

namespace fakeit {

/**
 * A composite MethodInvocationHandler that holds a list of ActionSequence objects.
 */
    template<typename R, typename ... arglist>
    class RecordedMethodBody : public MethodInvocationHandler<R, arglist...>, public ActualInvocationsSource {

        struct MatchedInvocationHandler : public MethodInvocationHandler<R, arglist...> {

            virtual ~MatchedInvocationHandler() = default;

            MatchedInvocationHandler(typename ActualInvocation<arglist...>::Matcher *matcher,
                                     MethodInvocationHandler<R, arglist...> *invocationHandler) :
                    _matcher{matcher}, _invocationHandler{invocationHandler} {
            }

            R handleMethodInvocation(arglist &... args) override {
                Destructible &destructable = *_invocationHandler;
                MethodInvocationHandler<R, arglist...> &invocationHandler = dynamic_cast<MethodInvocationHandler<R, arglist...> &>(destructable);
                return invocationHandler.handleMethodInvocation(args...);
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
                MethodInvocationHandler<R, arglist...> *invocationHandler) {
            return new MatchedInvocationHandler(invocationMatcher, invocationHandler);
        }

        MatchedInvocationHandler *getInvocationHandlerForActualArgs(ActualInvocation<arglist...> &invocation) {
            for (auto i = _invocationHandlers.rbegin(); i != _invocationHandlers.rend(); ++i) {
                std::shared_ptr<Destructible> curr = *i;
                Destructible &Destructable = *curr;
                MatchedInvocationHandler &im = asMatchedInvocationHandler(Destructable);
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

        virtual ~RecordedMethodBody() THROWS {
        }

        MethodInfo &getMethod() {
            return _method;
        }

        bool isOfMethod(MethodInfo &method) {
            //return &method == &_method;
            return method.id() == _method.id();
        }

        void addMethodInvocationHandler(typename ActualInvocation<arglist...>::Matcher *matcher,
                                        MethodInvocationHandler<R, arglist...> *invocationHandler) {
            auto *mock = buildMatchedInvocationHandler(matcher, invocationHandler);
            std::shared_ptr<Destructible> destructable{mock};
            _invocationHandlers.push_back(destructable);
        }

        void clear() {
            _invocationHandlers.clear();
            _actualInvocations.clear();
        }


        R handleMethodInvocation(arglist &... args) override {
            unsigned int ordinal = Invocation::nextInvocationOrdinal();
            MethodInfo &method = this->getMethod();
            auto actualInvoaction = new ActualInvocation<arglist...>(ordinal, method, args...);

            // ensure deletion if not added to actual invocations.
            std::shared_ptr<Destructible> actualInvoactionDtor{actualInvoaction};

            auto invocationHandler = getInvocationHandlerForActualArgs(*actualInvoaction);
            if (invocationHandler) {
                auto &matcher = invocationHandler->getMatcher();
                actualInvoaction->setActualMatcher(&matcher);
                _actualInvocations.push_back(actualInvoactionDtor);
                try {
                    return invocationHandler->handleMethodInvocation(args...);
                } catch (NoMoreRecordedActionException &) {
                }
            }

            UnexpectedMethodCallEvent event(UnexpectedType::Unmatched, *actualInvoaction);
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

        void getActualInvocations(std::unordered_set<Invocation *> &into) const {
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

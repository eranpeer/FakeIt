/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include <functional>
#include <utility>
#include <type_traits>
#include <tuple>
#include <memory>
#include <vector>
#include <unordered_set>
#include <set>
#include <iosfwd>

#include "fakeit/RecordedMethodBody.hpp"
#include "fakeit/StubbingProgress.hpp"
#include "fakeit/Sequence.hpp"
#include "fakeit/ActualInvocation.hpp"
#include "fakeit/ActualInvocationHandler.hpp"
#include "fakeit/EventHandler.hpp"
#include "fakeit/ActionSequence.hpp"
#include "fakeit/DomainObjects.hpp"
#include "fakeit/SpyingContext.hpp"
#include "fakeit/StubbingContext.hpp"
#include "fakeit/MatchersCollector.hpp"
#include "mockutils/type_utils.hpp"

namespace fakeit {
/**
 * Build recorded sequence and the matching criteria.
 * For example, for the following line:
 * When(Method(mock,foo)).Return(1).Return(2_Times(2)).Throw(e1);
 * The matching criteria is: Any invocation of mock.foo
 * The recorded sequence is: {Return(1), Return(2), Return(2), Throw(e1)}
 */
    template<typename R, typename ... arglist>
    class MethodMockingContext :
            public Sequence,                // For use in Verify(sequence1,...)... phrases.
            public ActualInvocationsSource, // For use in Using(source1,souece2,...) and VerifyNoOtherInvocations(source1,souece2...) phrases.
            public virtual StubbingContext<R, arglist...>, // For use in Fake & When phrases
            public virtual SpyingContext<R, arglist...>, // For use in Spy phrases
            private Invocation::Matcher {
    public:

        struct Context : Destructible {

            /**
             * Return the original method. not the mock.
             */
            virtual typename std::function<R(arglist&...)> getOriginalMethodCopyArgs() = 0;
            virtual typename std::function<R(arglist&...)> getOriginalMethodForwardArgs() = 0;

            virtual std::string getMethodName() = 0;

            virtual void addMethodInvocationHandler(typename ActualInvocation<arglist...>::Matcher *matcher,
                ActualInvocationHandler<R, arglist...> *invocationHandler) = 0;

            virtual void scanActualInvocations(const std::function<void(ActualInvocation<arglist...> &)> &scanner) = 0;

            virtual void setMethodDetails(std::string mockName, std::string methodName) = 0;

            virtual bool isOfMethod(MethodInfo &method) = 0;

            virtual ActualInvocationsSource &getInvolvedMock() = 0;
        };

    private:
        class Implementation {

            Context *_stubbingContext;
            ActionSequence<R, arglist...> *_recordedActionSequence;
            typename ActualInvocation<arglist...>::Matcher *_invocationMatcher;
            bool _commited;

            Context &getStubbingContext() const {
                return *_stubbingContext;
            }

        public:

            Implementation(Context *stubbingContext)
                    : _stubbingContext(stubbingContext),
                      _recordedActionSequence(new ActionSequence<R, arglist...>()),
                      _invocationMatcher
                              {
                                      new DefaultInvocationMatcher<arglist...>()}, _commited(false) {
            }

            ~Implementation() {
                delete _stubbingContext;
                if (!_commited) {
                    // no commit. delete the created objects.
                    delete _recordedActionSequence;
                    delete _invocationMatcher;
                }
            }

            ActionSequence<R, arglist...> &getRecordedActionSequence() {
                return *_recordedActionSequence;
            }

            std::string format() const {
                std::string s = getStubbingContext().getMethodName();
                s += _invocationMatcher->format();
                return s;
            }

            void getActualInvocations(std::unordered_set<Invocation *> &into) const {
                auto scanner = [&](ActualInvocation<arglist...> &a) {
                    if (_invocationMatcher->matches(a)) {
                        into.insert(&a);
                    }
                };
                getStubbingContext().scanActualInvocations(scanner);
            }

            /**
             * Used only by Verify phrase.
             */
            bool matches(Invocation &invocation) {
                MethodInfo &actualMethod = invocation.getMethod();
                if (!getStubbingContext().isOfMethod(actualMethod)) {
                    return false;
                }

                ActualInvocation<arglist...> &actualInvocation = dynamic_cast<ActualInvocation<arglist...> &>(invocation);
                return _invocationMatcher->matches(actualInvocation);
            }

            void commit() {
                getStubbingContext().addMethodInvocationHandler(_invocationMatcher, _recordedActionSequence);
                _commited = true;
            }

            void appendAction(Action<R, arglist...> *action) {
                getRecordedActionSequence().AppendDo(action);
            }

            void setMethodBodyByAssignment(std::function<R(const typename fakeit::test_arg<arglist>::type...)> method) {
                appendAction(new RepeatForever<R, arglist...>(method));
                commit();
            }

            void setMethodDetails(std::string mockName, std::string methodName) {
                getStubbingContext().setMethodDetails(mockName, methodName);
            }

            void getInvolvedMocks(std::vector<ActualInvocationsSource *> &into) const {
                into.push_back(&getStubbingContext().getInvolvedMock());
            }

            typename std::function<R(arglist &...)> getOriginalMethodCopyArgs() {
                return getStubbingContext().getOriginalMethodCopyArgs();
            }

            typename std::function<R(arglist &...)> getOriginalMethodForwardArgs() {
                return getStubbingContext().getOriginalMethodForwardArgs();
            }

            void setInvocationMatcher(typename ActualInvocation<arglist...>::Matcher *matcher) {
                delete _invocationMatcher;
                _invocationMatcher = matcher;
            }
        };

    protected:

        MethodMockingContext(Context *stubbingContext)
                : _impl{new Implementation(stubbingContext)} {
        }

        MethodMockingContext(MethodMockingContext &) = default;

        //we have to write move ctor by hand since VC 2013 doesn't support defaulted
        //move constructor and move assignment
        MethodMockingContext(MethodMockingContext &&other)
                : _impl(std::move(other._impl)) {
        }

        virtual ~MethodMockingContext() NO_THROWS { }

        std::string format() const override {
            return _impl->format();
        }

        unsigned int size() const override {
            return 1;
        }

        /**
         * Used only by Verify phrase.
         */
        void getInvolvedMocks(std::vector<ActualInvocationsSource *> &into) const override {
            _impl->getInvolvedMocks(into);
        }

        void getExpectedSequence(std::vector<Invocation::Matcher *> &into) const override {
            const Invocation::Matcher *b = this;
            Invocation::Matcher *c = const_cast<Invocation::Matcher *>(b);
            into.push_back(c);
        }

        /**
         * Used only by Verify phrase.
         */
        void getActualInvocations(std::unordered_set<Invocation *> &into) const override {
            _impl->getActualInvocations(into);
        }

        /**
         * Used only by Verify phrase.
         */
        bool matches(Invocation &invocation) override {
            return _impl->matches(invocation);
        }

        void commit() override {
            _impl->commit();
        }

        void setMethodDetails(std::string mockName, std::string methodName) {
            _impl->setMethodDetails(mockName, methodName);
        }

        void setMatchingCriteria(const std::function<bool(arglist &...)>& predicate) {
            typename ActualInvocation<arglist...>::Matcher *matcher{
                    new UserDefinedInvocationMatcher<arglist...>(predicate)};
            _impl->setInvocationMatcher(matcher);
        }

        void setMatchingCriteria(std::vector<Destructible *> &matchers) {
            typename ActualInvocation<arglist...>::Matcher *matcher{
                    new ArgumentsMatcherInvocationMatcher<arglist...>(matchers)};
            _impl->setInvocationMatcher(matcher);
        }

        /**
         * Used by Fake, Spy & When functors
         */
        void appendAction(Action<R, arglist...> *action) override {
            _impl->appendAction(action);
        }

        void setMethodBodyByAssignment(std::function<R(const typename fakeit::test_arg<arglist>::type...)> method) {
            _impl->setMethodBodyByAssignment(method);
        }

        template<class ...matcherCreators>
        typename std::enable_if< //
                sizeof...(matcherCreators) == sizeof...(arglist), void> //
        ::type setMatchingCriteria(matcherCreators &&... matcherCreator) {
            std::vector<Destructible *> matchers;

            MatchersCollector<0, arglist...> c(matchers);
            c.CollectMatchers(std::forward<matcherCreators>(matcherCreator)...);

            MethodMockingContext<R, arglist...>::setMatchingCriteria(matchers);
        }

    private:

        typename std::function<R(arglist&...)> getOriginalMethodCopyArgs() override {
            return _impl->getOriginalMethodCopyArgs();
        }

        typename std::function<R(arglist&...)> getOriginalMethodForwardArgs() override {
            return _impl->getOriginalMethodForwardArgs();
        }

        std::shared_ptr<Implementation> _impl;
    };

    template<typename R, typename ... arglist>
    class MockingContext :
            public MethodMockingContext<R, arglist...> {
        MockingContext &operator=(const MockingContext &) = delete;

    public:

        MockingContext(typename MethodMockingContext<R, arglist...>::Context *stubbingContext)
                : MethodMockingContext<R, arglist...>(stubbingContext) {
        }

        MockingContext(MockingContext &) = default;

        MockingContext(MockingContext &&other)
                : MethodMockingContext<R, arglist...>(std::move(other)) {
        }

        MockingContext<R, arglist...> &setMethodDetails(std::string mockName, std::string methodName) {
            MethodMockingContext<R, arglist...>::setMethodDetails(mockName, methodName);
            return *this;
        }

        template<class ...arg_matcher>
        MockingContext<R, arglist...> &Using(arg_matcher &&... arg_matchers) {
            MethodMockingContext<R, arglist...>::setMatchingCriteria(std::forward<arg_matcher>(arg_matchers)...);
            return *this;
        }

        MockingContext<R, arglist...> &Matching(const std::function<bool(arglist &...)>& matcher) {
            MethodMockingContext<R, arglist...>::setMatchingCriteria(matcher);
            return *this;
        }

        MockingContext<R, arglist...> &operator()(const arglist &... args) {
            MethodMockingContext<R, arglist...>::setMatchingCriteria(args...);
            return *this;
        }

        MockingContext<R, arglist...> &operator()(const std::function<bool(arglist &...)>& matcher) {
            MethodMockingContext<R, arglist...>::setMatchingCriteria(matcher);
            return *this;
        }

        void operator=(std::function<R(arglist &...)> method) {
            MethodMockingContext<R, arglist...>::setMethodBodyByAssignment(method);
        }

        template<typename U = R>
        typename std::enable_if<!std::is_reference<U>::value, void>::type operator=(const R &r) {
            auto method = [r](const typename fakeit::test_arg<arglist>::type...) -> R { return r; };
            MethodMockingContext<R, arglist...>::setMethodBodyByAssignment(method);
        }

        template<typename U = R>
        typename std::enable_if<std::is_reference<U>::value, void>::type operator=(const R &r) {
            auto method = [&r](const typename fakeit::test_arg<arglist>::type...) -> R { return r; };
            MethodMockingContext<R, arglist...>::setMethodBodyByAssignment(method);
        }
    };

    template<typename ... arglist>
    class MockingContext<void, arglist...> :
            public MethodMockingContext<void, arglist...> {
        MockingContext &operator=(const MockingContext &) = delete;

    public:

        MockingContext(typename MethodMockingContext<void, arglist...>::Context *stubbingContext)
                : MethodMockingContext<void, arglist...>(stubbingContext) {
        }

        MockingContext(MockingContext &) = default;

        MockingContext(MockingContext &&other)
                : MethodMockingContext<void, arglist...>(std::move(other)) {
        }

        MockingContext<void, arglist...> &setMethodDetails(std::string mockName, std::string methodName) {
            MethodMockingContext<void, arglist...>::setMethodDetails(mockName, methodName);
            return *this;
        }

        template<class ...arg_matcher>
        MockingContext<void, arglist...> &Using(arg_matcher &&... arg_matchers) {
            MethodMockingContext<void, arglist...>::setMatchingCriteria(std::forward<arg_matcher>(arg_matchers)...);
            return *this;
        }

        MockingContext<void, arglist...> &Matching(const std::function<bool(arglist &...)>& matcher) {
            MethodMockingContext<void, arglist...>::setMatchingCriteria(matcher);
            return *this;
        }

        MockingContext<void, arglist...> &operator()(const arglist &... args) {
            MethodMockingContext<void, arglist...>::setMatchingCriteria(args...);
            return *this;
        }

        MockingContext<void, arglist...> &operator()(const std::function<bool(arglist &...)>& matcher) {
            MethodMockingContext<void, arglist...>::setMatchingCriteria(matcher);
            return *this;
        }

        void operator=(std::function<void(arglist &...)> method) {
            MethodMockingContext<void, arglist...>::setMethodBodyByAssignment(method);
        }

    };

    class DtorMockingContext : public MethodMockingContext<void> {
    public:

        DtorMockingContext(MethodMockingContext<void>::Context *stubbingContext)
                : MethodMockingContext<void>(stubbingContext) {
        }

        DtorMockingContext(DtorMockingContext &other) : MethodMockingContext<void>(other) {
        }

        DtorMockingContext(DtorMockingContext &&other) : MethodMockingContext<void>(std::move(other)) {
        }

        void operator=(std::function<void()> method) {
            MethodMockingContext<void>::setMethodBodyByAssignment(method);
        }

        DtorMockingContext &setMethodDetails(std::string mockName, std::string methodName) {
            MethodMockingContext<void>::setMethodDetails(mockName, methodName);
            return *this;
        }
    };

}

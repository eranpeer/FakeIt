/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include <functional>
#include <type_traits>
#include <stdexcept>
#include <utility>

#include "mockutils/DefaultValue.hpp"
#include "mockutils/Macros.hpp"
#include "mockutils/type_utils.hpp"

#include "fakeit/FakeitExceptions.hpp"
#include "fakeit/ActualInvocation.hpp"
#include "fakeit/Quantifier.hpp"
#include "fakeit/Action.hpp"

namespace fakeit {

    template<typename R, typename ... arglist>
    struct MethodStubbingProgress {

        virtual ~MethodStubbingProgress() THROWS {
        }

        template<typename U = R>
        typename std::enable_if<!std::is_reference<U>::value, MethodStubbingProgress<R, arglist...> &>::type
        Return(const R &r) {
            return Do([r](const typename fakeit::test_arg<arglist>::type...) -> R { return r; });
        }

        template<typename U = R>
        typename std::enable_if<std::is_reference<U>::value, MethodStubbingProgress<R, arglist...> &>::type
        Return(const R &r) {
            return Do([&r](const typename fakeit::test_arg<arglist>::type...) -> R { return r; });
        }

        MethodStubbingProgress<R, arglist...> &
        Return(const Quantifier<R> &q) {
            const R &value = q.value;
            auto method = [value](const arglist &...) -> R { return value; };
            return DoImpl(new Repeat<R, arglist...>(method, q.quantity));
        }

        template<typename first, typename second, typename ... tail>
        MethodStubbingProgress<R, arglist...> &
        Return(const first &f, const second &s, const tail &... t) {
            Return(f);
            return Return(s, t...);
        }


        template<typename U = R>
        typename std::enable_if<!std::is_reference<U>::value, void>::type
        AlwaysReturn(const R &r) {
            return AlwaysDo([r](const typename fakeit::test_arg<arglist>::type...) -> R { return r; });
        }

        template<typename U = R>
        typename std::enable_if<std::is_reference<U>::value, void>::type
        AlwaysReturn(const R &r) {
            return AlwaysDo([&r](const typename fakeit::test_arg<arglist>::type...) -> R { return r; });
        }

        MethodStubbingProgress<R, arglist...> &
        Return() {
            return Do([](const typename fakeit::test_arg<arglist>::type...) -> R { return DefaultValue<R>::value(); });
        }

        void AlwaysReturn() {
            return AlwaysDo([](const typename fakeit::test_arg<arglist>::type...) -> R { return DefaultValue<R>::value(); });
        }

        template<typename E>
        MethodStubbingProgress<R, arglist...> &Throw(const E &e) {
            return Do([e](const typename fakeit::test_arg<arglist>::type...) -> R { throw e; });
        }

        template<typename E>
        MethodStubbingProgress<R, arglist...> &
        Throw(const Quantifier<E> &q) {
            const E &value = q.value;
            auto method = [value](const arglist &...) -> R { throw value; };
            return DoImpl(new Repeat<R, arglist...>(method, q.quantity));
        }

        template<typename first, typename second, typename ... tail>
        MethodStubbingProgress<R, arglist...> &
        Throw(const first &f, const second &s, const tail &... t) {
            Throw(f);
            return Throw(s, t...);
        }

        template<typename E>
        void AlwaysThrow(const E &e) {
            return AlwaysDo([e](const typename fakeit::test_arg<arglist>::type...) -> R { throw e; });
        }

        virtual MethodStubbingProgress<R, arglist...> &
            Do(std::function<R(const typename fakeit::test_arg<arglist>::type...)> method) {
            return DoImpl(new Repeat<R, arglist...>(method));
        }

        template<typename F>
        MethodStubbingProgress<R, arglist...> &
        Do(const Quantifier<F> &q) {
            return DoImpl(new Repeat<R, arglist...>(q.value, q.quantity));
        }

        template<typename first, typename second, typename ... tail>
        MethodStubbingProgress<R, arglist...> &
        Do(const first &f, const second &s, const tail &... t) {
            Do(f);
            return Do(s, t...);
        }

        virtual void AlwaysDo(std::function<R(const typename fakeit::test_arg<arglist>::type...)> method) {
            DoImpl(new RepeatForever<R, arglist...>(method));
        }

    protected:

        virtual MethodStubbingProgress<R, arglist...> &DoImpl(Action<R, arglist...> *action) = 0;

    private:
        MethodStubbingProgress &operator=(const MethodStubbingProgress &other) = delete;
    };


    template<typename ... arglist>
    struct MethodStubbingProgress<void, arglist...> {

        virtual ~MethodStubbingProgress() THROWS {
        }

        MethodStubbingProgress<void, arglist...> &Return() {
            auto lambda = [](const typename fakeit::test_arg<arglist>::type...) -> void {
                return DefaultValue<void>::value(); 
            };
            return Do(lambda);
        }

        virtual MethodStubbingProgress<void, arglist...> &Do(
            std::function<void(const typename fakeit::test_arg<arglist>::type...)> method) {
            return DoImpl(new Repeat<void, arglist...>(method));
        }


        void AlwaysReturn() {
            return AlwaysDo([](const typename fakeit::test_arg<arglist>::type...) -> void { return DefaultValue<void>::value(); });
        }

        MethodStubbingProgress<void, arglist...> &
        Return(const Quantifier<void> &q) {
            auto method = [](const arglist &...) -> void { return DefaultValue<void>::value(); };
            return DoImpl(new Repeat<void, arglist...>(method, q.quantity));
        }

        template<typename E>
        MethodStubbingProgress<void, arglist...> &Throw(const E &e) {
            return Do([e](const typename fakeit::test_arg<arglist>::type...) -> void { throw e; });
        }

        template<typename E>
        MethodStubbingProgress<void, arglist...> &
        Throw(const Quantifier<E> &q) {
            const E &value = q.value;
            auto method = [value](const typename fakeit::test_arg<arglist>::type...) -> void { throw value; };
            return DoImpl(new Repeat<void, arglist...>(method, q.quantity));
        }

        template<typename first, typename second, typename ... tail>
        MethodStubbingProgress<void, arglist...> &
        Throw(const first &f, const second &s, const tail &... t) {
            Throw(f);
            return Throw(s, t...);
        }

        template<typename E>
        void AlwaysThrow(const E e) {
            return AlwaysDo([e](const typename fakeit::test_arg<arglist>::type...) -> void { throw e; });
        }

           template<typename F>
        MethodStubbingProgress<void, arglist...> &
        Do(const Quantifier<F> &q) {
            return DoImpl(new Repeat<void, arglist...>(q.value, q.quantity));
        }

        template<typename first, typename second, typename ... tail>
        MethodStubbingProgress<void, arglist...> &
        Do(const first &f, const second &s, const tail &... t) {
            Do(f);
            return Do(s, t...);
        }

        virtual void AlwaysDo(std::function<void(const typename fakeit::test_arg<arglist>::type...)> method) {
            DoImpl(new RepeatForever<void, arglist...>(method));
        }

    protected:

        virtual MethodStubbingProgress<void, arglist...> &DoImpl(Action<void, arglist...> *action) = 0;

    private:
        MethodStubbingProgress &operator=(const MethodStubbingProgress &other) = delete;
    };


}

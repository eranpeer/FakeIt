/*
 * Action.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Jun 5, 2014
 */
#pragma once

#include <functional>
#include <atomic>
#include <tuple>
#include <type_traits>

#include "mockutils/DefaultValue.hpp"
#include "mockutils/Destructible.hpp"
#include "fakeit/FakeitExceptions.hpp"

namespace fakeit {

    template<typename R, typename ... arglist>
    struct Action : public Destructible {
        virtual ~Action() = default;

        virtual R invoke(const typename fakeit::production_arg<arglist>::type... args) = 0;

        virtual bool isDone() = 0;
    };

    template<typename R, typename ... arglist>
    struct Repeat : Action<R, arglist...> {
        virtual ~Repeat() = default;

        Repeat(std::function<R(typename fakeit::test_arg<arglist>::type...)> func) :
                f(func), times(1) {
        }

        Repeat(std::function<R(typename fakeit::test_arg<arglist>::type...)> func, long t) :
                f(func), times(t) {
        }

        virtual R invoke(typename fakeit::production_arg<arglist>::type... args) override {
            times--;
            return f(args...);
        }

        virtual bool isDone() override {
            return times == 0;
        }

    private:
        std::function<R(typename fakeit::test_arg<arglist>::type...)> f;
        long times;
    };

    template<typename R, typename ... arglist>
    struct RepeatForever : public Action<R, arglist...> {

        virtual ~RepeatForever() = default;

        RepeatForever(std::function<R(typename fakeit::test_arg<arglist>::type...)> func) :
                f(func) {
        }

        virtual R invoke(typename fakeit::production_arg<arglist>::type... args) override {
            return f(args...);
        }

        virtual bool isDone() override {
            return false;
        }

    private:
        std::function<R(typename fakeit::test_arg<arglist>::type...)> f;
    };

    template<typename R, typename ... arglist>
    struct ReturnDefaultValue : public Action<R, arglist...> {
        virtual ~ReturnDefaultValue() = default;

        virtual R invoke(const typename fakeit::production_arg<arglist>::type...) override {
            return DefaultValue<R>::value();
        }

        virtual bool isDone() override {
            return false;
        }
    };

    template<typename R, typename ... arglist>
    struct ReturnDelegateValue : public Action<R, arglist...> {

        ReturnDelegateValue(std::function<R(const typename fakeit::test_arg<arglist>::type...)> delegate) : _delegate(delegate) { }

        virtual ~ReturnDelegateValue() = default;

        virtual R invoke(const typename fakeit::production_arg<arglist>::type... args) override {
            return _delegate(args...);
        }

        virtual bool isDone() override {
            return false;
        }

    private:
        std::function<R(const typename fakeit::test_arg<arglist>::type...)> _delegate;
    };

}

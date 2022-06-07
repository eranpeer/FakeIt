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

    namespace helper
    {
        template <typename T, int N>
        struct ArgValue;

        template <int max_index, int tuple_index>
        struct ArgValidator;

        template<int arg_index, typename current_arg, typename ...T, int ...N, typename ... arglist>
        static void
        Assign(std::tuple<ArgValue<T, N>...> arg_vals, current_arg &&p, arglist &&... args);

        template<int N>
        struct ParamWalker;

    }  // namespace helper


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

        template<typename ... valuelist>
        MethodStubbingProgress<R, arglist...> &
        ReturnAndSet(R &&r, valuelist &&... arg_vals) {
            return Do(GetAssigner(std::forward<R>(r),
                    std::forward<valuelist>(arg_vals)...));
        }

        template<typename ... valuelist>
        void AlwaysReturnAndSet(R &&r, valuelist &&... arg_vals) {
            AlwaysDo(GetAssigner(std::forward<R>(r),
                std::forward<valuelist>(arg_vals)...));
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

        template<typename ... valuelist>
#if __cplusplus >= 201402L
        auto
#else
        std::function<R (typename fakeit::test_arg<arglist>::type...)>
#endif
        GetAssigner(R &&r, valuelist &&... arg_vals) {
            class Lambda {
            public:
                Lambda(R &&r, valuelist &&... arg_vals)
                    : vals_tuple{std::forward<R>(r), std::forward<valuelist>(arg_vals)...} {}

                R operator()(typename fakeit::test_arg<arglist>::type... args) {
                    helper::ParamWalker<sizeof...(valuelist)>::Assign(vals_tuple,
                        std::forward<arglist>(args)...);
                    return std::get<0>(vals_tuple);
                }

            private:
                ArgumentsTuple<R, valuelist...> vals_tuple;
            };

            return Lambda(std::forward<R>(r), std::forward<valuelist>(arg_vals)...);
        }

        template<typename ...T, int ...N>
#if __cplusplus >= 201402L
        auto
#else
        std::function<R (typename fakeit::test_arg<arglist>::type...)>
#endif
        GetAssigner(R &&r, helper::ArgValue<T, N>... arg_vals) {
            class Lambda {
            public:
                Lambda(R &&r, helper::ArgValue<T, N>... arg_vals)
                    : ret{std::forward<R>(r)}
                    , vals_tuple{std::forward<helper::ArgValue<T, N>>(arg_vals)...} {}

                R operator()(typename fakeit::test_arg<arglist>::type... args) {
                    helper::ArgValidator<sizeof...(arglist), sizeof...(T) - 1>::CheckPositions(vals_tuple);
                    helper::Assign<1>(vals_tuple, std::forward<arglist>(args)...);
                    return std::get<0>(ret);
                }

            private:
                std::tuple<R> ret;
                ArgumentsTuple<helper::ArgValue<T, N>...> vals_tuple;
            };

            return Lambda(std::forward<R>(r), std::forward<helper::ArgValue<T, N>>(arg_vals)...);
        }

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

        template<typename ... valuelist>
        MethodStubbingProgress<void, arglist...> &
        ReturnAndSet(valuelist &&... arg_vals) {
            return Do(GetAssigner(std::forward<valuelist>(arg_vals)...));
        }

        template<typename ... valuelist>
        void AlwaysReturnAndSet(valuelist &&... arg_vals) {
            AlwaysDo(GetAssigner(std::forward<valuelist>(arg_vals)...));
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

        template<typename ... valuelist>
#if __cplusplus >= 201402L
        auto
#else
        std::function<void (typename fakeit::test_arg<arglist>::type...)>
#endif
        GetAssigner(valuelist &&... arg_vals) {
            class Lambda {
            public:
                Lambda(valuelist &&... arg_vals)
                    : vals_tuple{std::forward<valuelist>(arg_vals)...} {}

                void operator()(typename fakeit::test_arg<arglist>::type... args) {
                    helper::ParamWalker<sizeof...(valuelist)>::Assign(vals_tuple,
                        std::forward<arglist>(args)...);
                }

            private:
                ArgumentsTuple<valuelist...> vals_tuple;
            };

            return Lambda(std::forward<valuelist>(arg_vals)...);
        }

        template<typename ...T, int ...N>
#if __cplusplus >= 201402L
        auto
#else
        std::function<void (typename fakeit::test_arg<arglist>::type...)>
#endif
        GetAssigner(helper::ArgValue<T, N>... arg_vals) {
            class Lambda {
            public:
                Lambda(helper::ArgValue<T, N>... arg_vals)
                    : vals_tuple{std::forward<helper::ArgValue<T, N>>(arg_vals)...} {}

                void operator()(typename fakeit::test_arg<arglist>::type... args) {
                    helper::ArgValidator<sizeof...(arglist), sizeof...(T) - 1>::CheckPositions(vals_tuple);
                    helper::Assign<1>(vals_tuple, std::forward<arglist>(args)...);
                }

            private:
                ArgumentsTuple<helper::ArgValue<T, N>...> vals_tuple;
            };

            return Lambda(std::forward<helper::ArgValue<T, N>>(arg_vals)...);
        }

    };


    namespace helper
    {
        template <typename T, int N>
        struct ArgValue
        {
            ArgValue(T &&v): value ( std::forward<T>(v) ) {}
            constexpr static int pos = N;
            T value;
        };

        template <int max_index, int tuple_index>
        struct ArgValidator
        {
            template <typename ...T, int ...N>
            static void CheckPositions(const std::tuple<ArgValue<T, N>...> arg_vals)
            {
#if __cplusplus >= 201402L
                static_assert(std::get<tuple_index>(arg_vals).pos <= max_index,
                    "Argument index out of range");
                ArgValidator<max_index, tuple_index - 1>::CheckPositions(arg_vals);
#else
                (void)arg_vals; // to suppress unused variable warning in C++11
#endif
            }
        };

        template <int max_index>
        struct ArgValidator<max_index, -1>
        {
            template <typename T>
            static void CheckPositions(T) {}
        };

        template <typename current_arg>
        typename std::enable_if<std::is_pointer<current_arg>::value,
            typename std::remove_pointer<current_arg>::type &>::type
        GetArg(current_arg &&t)
        {
            return *t;
        }

        template <typename current_arg>
        typename std::enable_if<!std::is_pointer<current_arg>::value, current_arg>::type
        GetArg(current_arg &&t)
        {
            return std::forward<current_arg>(t);
        }

        template<int N>
        struct ParamWalker {
            template<typename current_arg, typename ... valuelist, typename ... arglist>
            static void
            Assign(ArgumentsTuple<valuelist...> arg_vals, current_arg &&p, arglist&&... args) {
                ParamWalker<N - 1>::template Assign(arg_vals, std::forward<arglist>(args)...);
                GetArg(std::forward<current_arg>(p)) = std::get<sizeof...(valuelist) - N>(arg_vals);
            }
        };

        template<>
        struct ParamWalker<0> {
            template<typename ... valuelist, typename ... arglist>
            static void Assign(ArgumentsTuple<valuelist...>, arglist... ) {}
        };

        template<int arg_index, int check_index>
        struct ArgLocator {
            template<typename current_arg, typename ...T, int ...N>
            static void AssignArg(current_arg &&p, std::tuple<ArgValue<T, N>...> arg_vals) {
#if __cplusplus >= 201703L
                if constexpr (std::get<check_index>(arg_vals).pos == arg_index)
                    GetArg(std::forward<current_arg>(p)) = std::get<check_index>(arg_vals).value;
#else
                if (std::get<check_index>(arg_vals).pos == arg_index)
                    Set(std::forward<current_arg>(p), std::get<check_index>(arg_vals).value);
#endif
                else if (check_index > 0)
                    ArgLocator<arg_index, check_index - 1>::AssignArg(std::forward<current_arg>(p), arg_vals);
            }

#if __cplusplus < 201703L
        private:
            template<typename T, typename U>
            static
            typename std::enable_if<std::is_convertible<U, decltype(GetArg(std::declval<T>()))>::value, void>::type
            Set(T &&p, U &&v)
            {
                GetArg(std::forward<T>(p)) = v;
            }

            template<typename T, typename U>
            static
            typename std::enable_if<!std::is_convertible<U, decltype(GetArg(std::declval<T>()))>::value, void>::type
            Set(T &&, U &&)
            {
                throw std::logic_error("ReturnAndSet(): Invalid value type");
            }
#endif

        };

        template<int arg_index>
        struct ArgLocator<arg_index, -1> {
            template<typename current_arg, typename T>
            static void AssignArg(current_arg, T) {
            }
        };

        template<int arg_index, typename current_arg, typename ...T, int ...N, typename ... arglist>
        static void
        Assign(std::tuple<ArgValue<T, N>...> arg_vals, current_arg &&p, arglist &&... args) {
            ArgLocator<arg_index, sizeof...(N) - 1>::AssignArg(std::forward<current_arg>(p), arg_vals);
            Assign<arg_index + 1>(arg_vals, std::forward<arglist>(args)...);
        }

        template<int arg_index,  typename ... valuelist>
        static void Assign(std::tuple<valuelist...>) {}

    }  // namespace helper

    /*
     * Users might use our placeholders so that our operator<= is automatically
     * picked up using ADL. They might use std placeholders instead, but they
     * should make our operator<= from fakeit namespace visible to their code.
     */
    namespace placeholders
    {
        using namespace std::placeholders;

        template <typename PlaceHolder, typename ArgType,
            typename std::enable_if<static_cast<bool>(std::is_placeholder<PlaceHolder>::value), bool>::type = true>
        helper::ArgValue<ArgType, std::is_placeholder<PlaceHolder>::value>
        operator<=(PlaceHolder, ArgType &&arg)
        {
            return { std::forward<ArgType>(arg) };
        }

    }  // namespace placeholders

    using placeholders::operator <=;
}

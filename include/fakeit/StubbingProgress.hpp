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
    struct MethodStubbingProgress;

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

        template<typename R, typename ... arglist>
        struct BasicDoImpl {
            virtual ~BasicDoImpl() FAKEIT_THROWS {
            }

            virtual MethodStubbingProgress<R, arglist...>& Do(std::function<R(const typename fakeit::test_arg<arglist>::type...)> method) {
                return DoImpl(new Repeat<R, arglist...>(method));
            }

            virtual void AlwaysDo(std::function<R(const typename fakeit::test_arg<arglist>::type...)> method) {
                DoImpl(new RepeatForever<R, arglist...>(method));
            }

        protected:
            virtual MethodStubbingProgress<R, arglist...>& DoImpl(Action<R, arglist...> *action) = 0;
        };

        template<typename R, bool RIsARef, typename ... arglist>
        struct BasicReturnImpl;

        // If R is a reference.
        template<typename R, typename ... arglist>
        struct BasicReturnImpl<R, true, arglist...> : public BasicDoImpl<R, arglist...> {
            using BasicDoImpl<R, arglist...>::Do;
            using BasicDoImpl<R, arglist...>::AlwaysDo;

            MethodStubbingProgress<R, arglist...>& Return(const R& r) {
                return Do([&r](const typename fakeit::test_arg<arglist>::type...) -> R { return r; });
            }

            // The std::enable_if is only there to disambiguate with the deprecated version of .Return<type>(val), and
            // can be removed once that deprecated version is removed.
            template <typename U = R, typename std::enable_if<std::is_reference<U>::value, bool>::type = true>
            MethodStubbingProgress<R, arglist...>& Return(fk_remove_cvref_t<R>&& r) {
                static_assert(sizeof(U) != sizeof(U), "Return() cannot take an rvalue references for functions returning a reference because it would make it dangling, use ReturnValCapt() instead.");
                return Return(r); // Only written to silence warning about not returning from a non-void function, but will never be executed.
            }

            void AlwaysReturn(const R &r) {
                return AlwaysDo([&r](const typename fakeit::test_arg<arglist>::type...) -> R { return r; });
            }

            // The std::enable_if is only there to disambiguate with the deprecated version of .AlwaysReturn<type>(val), and
            // can be removed once that deprecated version is removed.
            template <typename U = R, typename std::enable_if<std::is_reference<U>::value, bool>::type = true>
            void AlwaysReturn(fk_remove_cvref_t<R>&&) {
                static_assert(sizeof(U) != sizeof(U), "AlwaysReturn() cannot take an rvalue references for functions returning a reference because it would make it dangling, use AlwaysReturnValCapt() instead.");
            }

            template<typename T>
            MethodStubbingProgress<R, arglist...>& ReturnValCapt(T&& r) {
                // If a ref to T can be cast to a ref to R, then store T.
                // Otherwise, create an object R constructed from the received T and store it.
                using StoredType = typename std::conditional<
                    std::is_constructible<fk_remove_cvref_t<R>&, fk_remove_cvref_t<T>&>::value,
                    fk_remove_cvref_t<T>,
                    fk_remove_cvref_t<R>>::type;
                auto store = std::make_shared<StoredType>(std::forward<T>(r));
                return Do([store](const typename fakeit::test_arg<arglist>::type...) mutable -> R {
                    return std::forward<R>(*store);
                });
            }

            template<typename T>
            void AlwaysReturnValCapt(T&& r) {
                // If a ref to T can be cast to a ref to R, then store T.
                // Otherwise, create an object R constructed from the received T and store it.
                using StoredType = typename std::conditional<
                    std::is_constructible<fk_remove_cvref_t<R>&, fk_remove_cvref_t<T>&>::value,
                    fk_remove_cvref_t<T>,
                    fk_remove_cvref_t<R>>::type;
                auto store = std::make_shared<StoredType>(std::forward<T>(r));
                return AlwaysDo([store](const typename fakeit::test_arg<arglist>::type...) mutable -> R {
                    return std::forward<R>(*store);
                });
            }
        };

        // If R is not a reference.
        template<typename R, typename ... arglist>
        struct BasicReturnImpl<R, false, arglist...> : public BasicDoImpl<R, arglist...> {
            using BasicDoImpl<R, arglist...>::Do;
            using BasicDoImpl<R, arglist...>::AlwaysDo;

            MethodStubbingProgress<R, arglist...>& Return(const R& r) {
                return Do([r](const typename fakeit::test_arg<arglist>::type...) -> R { return r; });
            }

            MethodStubbingProgress<R, arglist...>& Return(R&& r) {
                auto store = std::make_shared<R>(std::move(r)); // work around for lack of move_only_funciton( C++23) - move into a shared_ptr which we can copy.
                return Do([store](const typename fakeit::test_arg<arglist>::type...) mutable -> R {
                    return std::move(*store);
                });
            }

            void AlwaysReturn(const R &r) {
                return AlwaysDo([r](const typename fakeit::test_arg<arglist>::type...) -> R { return r; });
            }

            MethodStubbingProgress<R, arglist...>& ReturnValCapt(const R& r) {
                return Return(r);
            }

            MethodStubbingProgress<R, arglist...>& ReturnValCapt(R&& r) {
                return Return(std::move(r));
            }

            void AlwaysReturnValCapt(const R &r) {
                return AlwaysReturn(r);
            }
        };

        template<typename R, typename ... arglist>
        using BasicReturnImplHelper = BasicReturnImpl<R, std::is_reference<R>::value, arglist...>;
    }  // namespace helper


    template<typename R, typename ... arglist>
    struct MethodStubbingProgress : public helper::BasicReturnImplHelper<R, arglist...> {

    protected:
        using helper::BasicReturnImplHelper<R, arglist...>::DoImpl;

    public:
        using helper::BasicReturnImplHelper<R, arglist...>::Do;
        using helper::BasicReturnImplHelper<R, arglist...>::AlwaysDo;
        using helper::BasicReturnImplHelper<R, arglist...>::Return;
        using helper::BasicReturnImplHelper<R, arglist...>::AlwaysReturn;

        // DEPRECATED: This should ideally be removed, it allows writing .Return<std::string>("ok") when a function
        // returns "const std::string&" (for example) to have the same behavior has .ReturnValCapt("ok"). But it is prone
        // to errors (because you have to specify the type). .ReturnValCapt("ok") is superior and should be used instead.
        template<typename TypeUsedToForceCapture, typename RealType, typename std::enable_if<!std::is_reference<TypeUsedToForceCapture>::value, bool>::type = true>
        MethodStubbingProgress<R, arglist...>& Return(RealType&& ret) {
            return this->ReturnValCapt(TypeUsedToForceCapture(std::forward<RealType>(ret)));
        }

        // DEPRECATED: This should ideally be removed, it allows writing .AlwaysReturn<std::string>("ok") when a function
        // returns "const std::string&" (for example) to have the same behavior has .AlwaysReturnValCapt("ok"). But it is prone
        // to errors (because you have to specify the type). .AlwaysReturnValCapt("ok") is superior and should be used instead.
        template<typename TypeUsedToForceCapture, typename RealType, typename std::enable_if<!std::is_reference<TypeUsedToForceCapture>::value, bool>::type = true>
        void AlwaysReturn(RealType&& ret) {
            return this->AlwaysReturnValCapt(TypeUsedToForceCapture(std::forward<RealType>(ret)));
        }

        MethodStubbingProgress<R, arglist...> &
        Return(const Quantifier<R> &q) {
            const R &value = q.value;
            auto method = [value](const arglist &...) -> R { return value; };
            return DoImpl(new Repeat<R, arglist...>(method, q.quantity));
        }

        template<typename First, typename Second, typename... Tail>
        MethodStubbingProgress<R, arglist...> &
        Return(First&& f, Second&& s, Tail&&... t) {
            Return(std::forward<First>(f));
            return Return(std::forward<Second>(s), std::forward<Tail>(t)...);
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

    private:
        MethodStubbingProgress &operator=(const MethodStubbingProgress &other) = delete;

        template<typename ... valuelist>
#if FAKEIT_CPLUSPLUS >= 201402L
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
#if FAKEIT_CPLUSPLUS >= 201402L
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

        virtual ~MethodStubbingProgress() FAKEIT_THROWS {
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
#if FAKEIT_CPLUSPLUS >= 201402L
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
#if FAKEIT_CPLUSPLUS >= 201402L
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
#if FAKEIT_CPLUSPLUS >= 201402L && !defined(_WIN32)
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
                ParamWalker<N - 1>::Assign(arg_vals, std::forward<arglist>(args)...);
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
#if FAKEIT_CPLUSPLUS >= 201703L && !defined (_WIN32)
                if constexpr (std::get<check_index>(arg_vals).pos == arg_index)
                    GetArg(std::forward<current_arg>(p)) = std::get<check_index>(arg_vals).value;
#else
                if (std::get<check_index>(arg_vals).pos == arg_index)
                    Set(std::forward<current_arg>(p), std::get<check_index>(arg_vals).value);
#endif
                else if (check_index > 0)
                    ArgLocator<arg_index, check_index - 1>::AssignArg(std::forward<current_arg>(p), arg_vals);
            }

#if FAKEIT_CPLUSPLUS < 201703L || defined (_WIN32)
        private:
            template<typename T, typename U>
            static
            typename std::enable_if<std::is_assignable<decltype(GetArg(std::declval<T>())), U>::value, void>::type
            Set(T &&p, U &&v)
            {
                GetArg(std::forward<T>(p)) = v;
            }

            template<typename T, typename U>
            static
            typename std::enable_if<!std::is_assignable<decltype(GetArg(std::declval<T>())), U>::value, void>::type
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

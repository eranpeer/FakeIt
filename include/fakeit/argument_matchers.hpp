/*
 * argument_matchers.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Jan 12, 2015
 */
#pragma once

#include <cmath>
#include <cstring>

#include "mockutils/type_utils.hpp"

namespace fakeit {

    struct IMatcher : Destructible {
        ~IMatcher() = default;
        virtual std::string format() const = 0;
    };

    template<typename ActualT>
    struct TypedMatcher : IMatcher {
        virtual bool matches(const ActualT &actual) const = 0;
    };

    template<typename ExpectedTRef>
    struct ComparisonMatcherCreatorBase {
        using ExpectedT = typename naked_type<ExpectedTRef>::type;

        ExpectedTRef _expectedRef;

        template <typename T>
        ComparisonMatcherCreatorBase(T &&expectedRef)
                : _expectedRef(std::forward<T>(expectedRef)) {
        }

        template <typename ActualT, typename = ExpectedT, typename = void>
        struct MatcherBase : public TypedMatcher<ActualT> {
            const ExpectedT _expected;

            MatcherBase(ExpectedTRef expected)
                    : _expected{std::forward<ExpectedTRef>(expected)} {
            }
        };

        template <typename ActualT, typename U>
        struct MatcherBase<ActualT, U, typename std::enable_if<std::is_same<U, ExpectedT>::value && std::is_array<U>::value>::type> : public TypedMatcher<ActualT> {
            ExpectedT _expected;

            MatcherBase(ExpectedTRef expected) {
                std::memcpy(_expected, expected, sizeof(_expected));
            }
        };
    };

    namespace internal {
        struct AnyMatcherCreator{
            template <typename ActualT>
            struct IsTypeCompatible : std::true_type {};

            template<typename ActualT>
            TypedMatcher<ActualT> *createMatcher() const {
                struct Matcher : public TypedMatcher<ActualT> {
                    bool matches(const ActualT &) const override {
                        return true;
                    }

                    std::string format() const override {
                        return "Any";
                    }
                };

                return new Matcher();
            }
        };

        template<typename ExpectedTRef>
        struct EqMatcherCreator : public ComparisonMatcherCreatorBase<ExpectedTRef> {
            using ExpectedT = typename ComparisonMatcherCreatorBase<ExpectedTRef>::ExpectedT;

            template <typename ActualT, typename = void>
            struct IsTypeCompatible : std::false_type {};

            template <typename ActualT>
            struct IsTypeCompatible<ActualT, fk_void_t<decltype(std::declval<ActualT>() == std::declval<ExpectedT>())>> : std::true_type {};

            using ComparisonMatcherCreatorBase<ExpectedTRef>::ComparisonMatcherCreatorBase;

            template<typename ActualT>
            TypedMatcher<ActualT> *createMatcher() const {
                struct Matcher : public ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT> {
                    using ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT>::MatcherBase;

                    virtual std::string format() const override {
                        return TypeFormatter<ExpectedT>::format(this->_expected);
                    }

                    virtual bool matches(const ActualT &actual) const override {
                        return actual == this->_expected;
                    }
                };

                return new Matcher(std::forward<ExpectedTRef>(this->_expectedRef));
            }
        };

        template<typename ExpectedTRef>
        struct GtMatcherCreator : public ComparisonMatcherCreatorBase<ExpectedTRef> {
            using ExpectedT = typename ComparisonMatcherCreatorBase<ExpectedTRef>::ExpectedT;

            template <typename ActualT, typename = void>
            struct IsTypeCompatible : std::false_type {};

            template <typename ActualT>
            struct IsTypeCompatible<ActualT, fk_void_t<decltype(std::declval<ActualT>() > std::declval<ExpectedT>())>> : std::true_type {};

            using ComparisonMatcherCreatorBase<ExpectedTRef>::ComparisonMatcherCreatorBase;

            template<typename ActualT>
            TypedMatcher<ActualT> *createMatcher() const {
                struct Matcher : public ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT> {
                    using ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT>::MatcherBase;

                    virtual std::string format() const override {
                        return std::string(">") + TypeFormatter<ExpectedT>::format(this->_expected);
                    }

                    virtual bool matches(const ActualT &actual) const override {
                        return actual > this->_expected;
                    }
                };

                return new Matcher(std::forward<ExpectedTRef>(this->_expectedRef));
            }
        };

        template<typename ExpectedTRef>
        struct GeMatcherCreator : public ComparisonMatcherCreatorBase<ExpectedTRef> {
            using ExpectedT = typename ComparisonMatcherCreatorBase<ExpectedTRef>::ExpectedT;

            template <typename ActualT, typename = void>
            struct IsTypeCompatible : std::false_type {};

            template <typename ActualT>
            struct IsTypeCompatible<ActualT, fk_void_t<decltype(std::declval<ActualT>() >= std::declval<ExpectedT>())>> : std::true_type {};

            using ComparisonMatcherCreatorBase<ExpectedTRef>::ComparisonMatcherCreatorBase;

            template<typename ActualT>
            TypedMatcher<ActualT> *createMatcher() const {
                struct Matcher : public ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT> {
                    using ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT>::MatcherBase;

                    virtual std::string format() const override {
                        return std::string(">=") + TypeFormatter<ExpectedT>::format(this->_expected);
                    }

                    virtual bool matches(const ActualT &actual) const override {
                        return actual >= this->_expected;
                    }
                };

                return new Matcher(std::forward<ExpectedTRef>(this->_expectedRef));
            }
        };

        template<typename ExpectedTRef>
        struct LtMatcherCreator : public ComparisonMatcherCreatorBase<ExpectedTRef> {
            using ExpectedT = typename ComparisonMatcherCreatorBase<ExpectedTRef>::ExpectedT;

            template <typename ActualT, typename = void>
            struct IsTypeCompatible : std::false_type {};

            template <typename ActualT>
            struct IsTypeCompatible<ActualT, fk_void_t<decltype(std::declval<ActualT>() < std::declval<ExpectedT>())>> : std::true_type {};

            using ComparisonMatcherCreatorBase<ExpectedTRef>::ComparisonMatcherCreatorBase;

            template<typename ActualT>
            TypedMatcher<ActualT> *createMatcher() const {
                struct Matcher : public ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT> {
                    using ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT>::MatcherBase;

                    virtual std::string format() const override {
                        return std::string("<") + TypeFormatter<ExpectedT>::format(this->_expected);
                    }

                    virtual bool matches(const ActualT &actual) const override {
                        return actual < this->_expected;
                    }
                };

                return new Matcher(std::forward<ExpectedTRef>(this->_expectedRef));
            }
        };

        template<typename ExpectedTRef>
        struct LeMatcherCreator : public ComparisonMatcherCreatorBase<ExpectedTRef> {
            using ExpectedT = typename ComparisonMatcherCreatorBase<ExpectedTRef>::ExpectedT;

            template <typename ActualT, typename = void>
            struct IsTypeCompatible : std::false_type {};

            template <typename ActualT>
            struct IsTypeCompatible<ActualT, fk_void_t<decltype(std::declval<ActualT>() <= std::declval<ExpectedT>())>> : std::true_type {};

            using ComparisonMatcherCreatorBase<ExpectedTRef>::ComparisonMatcherCreatorBase;

            template<typename ActualT>
            TypedMatcher<ActualT> *createMatcher() const {
                struct Matcher : public ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT> {
                    using ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT>::MatcherBase;

                    virtual std::string format() const override {
                        return std::string("<=") + TypeFormatter<ExpectedT>::format(this->_expected);
                    }

                    virtual bool matches(const ActualT &actual) const override {
                        return actual <= this->_expected;
                    }
                };

                return new Matcher(std::forward<ExpectedTRef>(this->_expectedRef));
            }
        };

        template<typename ExpectedTRef>
        struct NeMatcherCreator : public ComparisonMatcherCreatorBase<ExpectedTRef> {
            using ExpectedT = typename ComparisonMatcherCreatorBase<ExpectedTRef>::ExpectedT;

            template <typename ActualT, typename = void>
            struct IsTypeCompatible : std::false_type {};

            template <typename ActualT>
            struct IsTypeCompatible<ActualT, fk_void_t<decltype(std::declval<ActualT>() != std::declval<ExpectedT>())>> : std::true_type {};

            using ComparisonMatcherCreatorBase<ExpectedTRef>::ComparisonMatcherCreatorBase;

            template<typename ActualT>
            TypedMatcher<ActualT> *createMatcher() const {
                struct Matcher : public ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT> {
                    using ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT>::MatcherBase;

                    virtual std::string format() const override {
                        return std::string("!=") + TypeFormatter<ExpectedT>::format(this->_expected);
                    }

                    virtual bool matches(const ActualT &actual) const override {
                        return actual != this->_expected;
                    }
                };

                return new Matcher(std::forward<ExpectedTRef>(this->_expectedRef));
            }
        };

        template <typename ExpectedTRef>
        struct StrEqMatcherCreator : public ComparisonMatcherCreatorBase<ExpectedTRef> {
            using ExpectedT = typename ComparisonMatcherCreatorBase<ExpectedTRef>::ExpectedT;

            template <typename ActualT, typename = void>
            struct IsTypeCompatible : std::false_type {};

            template <typename ActualT>
            struct IsTypeCompatible<ActualT, fk_void_t<decltype(strcmp(std::declval<ActualT>(), std::declval<const char*>()))>> : std::true_type {};

            using ComparisonMatcherCreatorBase<ExpectedTRef>::ComparisonMatcherCreatorBase;

            template<typename ActualT>
            TypedMatcher<ActualT> *createMatcher() const {
                struct Matcher : public ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT> {
                    using ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT>::MatcherBase;

                    virtual std::string format() const override {
                        return TypeFormatter<ExpectedT>::format(this->_expected);
                    }

                    virtual bool matches(const ActualT &actual) const override {
                        return std::strcmp(actual, this->_expected.c_str()) == 0;
                    }
                };

                return new Matcher(std::forward<ExpectedTRef>(this->_expectedRef));
            }
        };

        template <typename ExpectedTRef>
        struct StrGtMatcherCreator : public ComparisonMatcherCreatorBase<ExpectedTRef> {
            using ExpectedT = typename ComparisonMatcherCreatorBase<ExpectedTRef>::ExpectedT;

            template <typename ActualT, typename = void>
            struct IsTypeCompatible : std::false_type {};

            template <typename ActualT>
            struct IsTypeCompatible<ActualT, fk_void_t<decltype(strcmp(std::declval<ActualT>(), std::declval<const char*>()))>> : std::true_type {};

            using ComparisonMatcherCreatorBase<ExpectedTRef>::ComparisonMatcherCreatorBase;

            template<typename ActualT>
            TypedMatcher<ActualT> *createMatcher() const {
                struct Matcher : public ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT> {
                    using ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT>::MatcherBase;

                    virtual std::string format() const override {
                        return std::string(">") + TypeFormatter<ExpectedT>::format(this->_expected);
                    }

                    virtual bool matches(const ActualT &actual) const override {
                        return std::strcmp(actual, this->_expected.c_str()) > 0;
                    }
                };

                return new Matcher(std::forward<ExpectedTRef>(this->_expectedRef));
            }
        };

        template <typename ExpectedTRef>
        struct StrGeMatcherCreator : public ComparisonMatcherCreatorBase<ExpectedTRef> {
            using ExpectedT = typename ComparisonMatcherCreatorBase<ExpectedTRef>::ExpectedT;

            template <typename ActualT, typename = void>
            struct IsTypeCompatible : std::false_type {};

            template <typename ActualT>
            struct IsTypeCompatible<ActualT, fk_void_t<decltype(strcmp(std::declval<ActualT>(), std::declval<const char*>()))>> : std::true_type {};

            using ComparisonMatcherCreatorBase<ExpectedTRef>::ComparisonMatcherCreatorBase;

            template<typename ActualT>
            TypedMatcher<ActualT> *createMatcher() const {
                struct Matcher : public ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT> {
                    using ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT>::MatcherBase;

                    virtual std::string format() const override {
                        return std::string(">=") + TypeFormatter<ExpectedT>::format(this->_expected);
                    }

                    virtual bool matches(const ActualT &actual) const override {
                        return std::strcmp(actual, this->_expected.c_str()) >= 0;
                    }
                };

                return new Matcher(std::forward<ExpectedTRef>(this->_expectedRef));
            }
        };

        template <typename ExpectedTRef>
        struct StrLtMatcherCreator : public ComparisonMatcherCreatorBase<ExpectedTRef> {
            using ExpectedT = typename ComparisonMatcherCreatorBase<ExpectedTRef>::ExpectedT;

            template <typename ActualT, typename = void>
            struct IsTypeCompatible : std::false_type {};

            template <typename ActualT>
            struct IsTypeCompatible<ActualT, fk_void_t<decltype(strcmp(std::declval<ActualT>(), std::declval<const char*>()))>> : std::true_type {};

            using ComparisonMatcherCreatorBase<ExpectedTRef>::ComparisonMatcherCreatorBase;

            template<typename ActualT>
            TypedMatcher<ActualT> *createMatcher() const {
                struct Matcher : public ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT> {
                    using ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT>::MatcherBase;

                    virtual std::string format() const override {
                        return std::string("<") + TypeFormatter<ExpectedT>::format(this->_expected);
                    }

                    virtual bool matches(const ActualT &actual) const override {
                        return std::strcmp(actual, this->_expected.c_str()) < 0;
                    }
                };

                return new Matcher(std::forward<ExpectedTRef>(this->_expectedRef));
            }
        };

        template <typename ExpectedTRef>
        struct StrLeMatcherCreator : public ComparisonMatcherCreatorBase<ExpectedTRef> {
            using ExpectedT = typename ComparisonMatcherCreatorBase<ExpectedTRef>::ExpectedT;

            template <typename ActualT, typename = void>
            struct IsTypeCompatible : std::false_type {};

            template <typename ActualT>
            struct IsTypeCompatible<ActualT, fk_void_t<decltype(strcmp(std::declval<ActualT>(), std::declval<const char*>()))>> : std::true_type {};

            using ComparisonMatcherCreatorBase<ExpectedTRef>::ComparisonMatcherCreatorBase;

            template<typename ActualT>
            TypedMatcher<ActualT> *createMatcher() const {
                struct Matcher : public ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT> {
                    using ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT>::MatcherBase;

                    virtual std::string format() const override {
                        return std::string("<=") + TypeFormatter<ExpectedT>::format(this->_expected);
                    }

                    virtual bool matches(const ActualT &actual) const override {
                        return std::strcmp(actual, this->_expected.c_str()) <= 0;
                    }
                };

                return new Matcher(std::forward<ExpectedTRef>(this->_expectedRef));
            }
        };

        template <typename ExpectedTRef>
        struct StrNeMatcherCreator : public ComparisonMatcherCreatorBase<ExpectedTRef> {
            using ExpectedT = typename ComparisonMatcherCreatorBase<ExpectedTRef>::ExpectedT;

            template <typename ActualT, typename = void>
            struct IsTypeCompatible : std::false_type {};

            template <typename ActualT>
            struct IsTypeCompatible<ActualT, fk_void_t<decltype(strcmp(std::declval<ActualT>(), std::declval<const char*>()))>> : std::true_type {};

            using ComparisonMatcherCreatorBase<ExpectedTRef>::ComparisonMatcherCreatorBase;

            template<typename ActualT>
            TypedMatcher<ActualT> *createMatcher() const {
                struct Matcher : public ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT> {
                    using ComparisonMatcherCreatorBase<ExpectedTRef>::template MatcherBase<ActualT>::MatcherBase;

                    virtual std::string format() const override {
                        return std::string("!=") + TypeFormatter<ExpectedT>::format(this->_expected);
                    }

                    virtual bool matches(const ActualT &actual) const override {
                        return std::strcmp(actual, this->_expected.c_str()) != 0;
                    }
                };

                return new Matcher(std::forward<ExpectedTRef>(this->_expectedRef));
            }
        };

        template<typename ExpectedTRef, typename ExpectedMarginTRef>
        struct ApproxEqCreator {
            using ExpectedT = typename naked_type<ExpectedTRef>::type;
            using ExpectedMarginT = typename naked_type<ExpectedMarginTRef>::type;

            template <typename ActualT, typename = void>
            struct IsTypeCompatible : std::false_type {};

            template <typename ActualT>
            struct IsTypeCompatible<ActualT, fk_void_t<decltype(std::abs(std::declval<ActualT>() - std::declval<ExpectedT>()) <= std::declval<ExpectedMarginT>())>> : std::true_type {};

            ExpectedTRef _expectedRef;
            ExpectedMarginTRef _expectedMarginRef;

            template <typename T, typename U>
            ApproxEqCreator(T &&expectedRef, U &&expectedMarginRef)
                    : _expectedRef(std::forward<T>(expectedRef))
                    , _expectedMarginRef(std::forward<U>(expectedMarginRef)) {
            }

            template<typename ActualT>
            TypedMatcher<ActualT> *createMatcher() const {
                struct Matcher : public TypedMatcher<ActualT> {
                    const ExpectedT _expected;
                    const ExpectedMarginT _expectedMargin;

                    Matcher(ExpectedTRef expected, ExpectedMarginTRef expectedMargin)
                            : _expected{std::forward<ExpectedTRef>(expected)}
                            , _expectedMargin{std::forward<ExpectedMarginTRef>(expectedMargin)} {
                    }

                    virtual std::string format() const override {
                        return TypeFormatter<ExpectedT>::format(this->_expected) + std::string("+/-") + TypeFormatter<ExpectedMarginT>::format(this->_expectedMargin);
                    }

                    virtual bool matches(const ActualT &actual) const override {
                        return std::abs(actual - this->_expected) <= this->_expectedMargin;
                    }
                };

                return new Matcher(std::forward<ExpectedTRef>(this->_expectedRef), std::forward<ExpectedMarginTRef>(this->_expectedMarginRef));
            }
        };
    }

    struct AnyMatcher {
    } static _;

    template <typename T>
    internal::AnyMatcherCreator Any() {
        static_assert(sizeof(T) >= 0, "To maintain backward compatibility, this function takes an useless template argument.");
        internal::AnyMatcherCreator mc;
        return mc;
    }

    inline internal::AnyMatcherCreator Any() {
        internal::AnyMatcherCreator mc;
        return mc;
    }

    template<typename T>
    internal::EqMatcherCreator<T&&> Eq(T &&arg) {
        internal::EqMatcherCreator<T&&> mc(std::forward<T>(arg));
        return mc;
    }

    template<typename T>
    internal::GtMatcherCreator<T&&> Gt(T &&arg) {
        internal::GtMatcherCreator<T&&> mc(std::forward<T>(arg));
        return mc;
    }

    template<typename T>
    internal::GeMatcherCreator<T&&> Ge(T &&arg) {
        internal::GeMatcherCreator<T&&> mc(std::forward<T>(arg));
        return mc;
    }

    template<typename T>
    internal::LtMatcherCreator<T&&> Lt(T &&arg) {
        internal::LtMatcherCreator<T&&> mc(std::forward<T>(arg));
        return mc;
    }

    template<typename T>
    internal::LeMatcherCreator<T&&> Le(T &&arg) {
        internal::LeMatcherCreator<T&&> mc(std::forward<T>(arg));
        return mc;
    }

    template<typename T>
    internal::NeMatcherCreator<T&&> Ne(T &&arg) {
        internal::NeMatcherCreator<T&&> mc(std::forward<T>(arg));
        return mc;
    }

    inline internal::StrEqMatcherCreator<std::string&&> StrEq(std::string&& arg) {
        internal::StrEqMatcherCreator<std::string&&> mc(std::move(arg));
        return mc;
    }

    inline internal::StrEqMatcherCreator<const std::string&> StrEq(const std::string& arg) {
        internal::StrEqMatcherCreator<const std::string&> mc(arg);
        return mc;
    }

    inline internal::StrGtMatcherCreator<std::string&&> StrGt(std::string&& arg) {
        internal::StrGtMatcherCreator<std::string&&> mc(std::move(arg));
        return mc;
    }

    inline internal::StrGtMatcherCreator<const std::string&> StrGt(const std::string& arg) {
        internal::StrGtMatcherCreator<const std::string&> mc(arg);
        return mc;
    }

    inline internal::StrGeMatcherCreator<std::string&&> StrGe(std::string&& arg) {
        internal::StrGeMatcherCreator<std::string&&> mc(std::move(arg));
        return mc;
    }

    inline internal::StrGeMatcherCreator<const std::string&> StrGe(const std::string& arg) {
        internal::StrGeMatcherCreator<const std::string&> mc(arg);
        return mc;
    }

    inline internal::StrLtMatcherCreator<std::string&&> StrLt(std::string&& arg) {
        internal::StrLtMatcherCreator<std::string&&> mc(std::move(arg));
        return mc;
    }

    inline internal::StrLtMatcherCreator<const std::string&> StrLt(const std::string& arg) {
        internal::StrLtMatcherCreator<const std::string&> mc(arg);
        return mc;
    }

    inline internal::StrLeMatcherCreator<std::string&&> StrLe(std::string&& arg) {
        internal::StrLeMatcherCreator<std::string&&> mc(std::move(arg));
        return mc;
    }

    inline internal::StrLeMatcherCreator<const std::string&> StrLe(const std::string& arg) {
        internal::StrLeMatcherCreator<const std::string&> mc(arg);
        return mc;
    }

    inline internal::StrNeMatcherCreator<std::string&&> StrNe(std::string&& arg) {
        internal::StrNeMatcherCreator<std::string&&> mc(std::move(arg));
        return mc;
    }

    inline internal::StrNeMatcherCreator<const std::string&> StrNe(const std::string& arg) {
        internal::StrNeMatcherCreator<const std::string&> mc(arg);
        return mc;
    }

    template<typename T, typename U,
        typename std::enable_if<std::is_arithmetic<typename naked_type<T>::type>::value, int>::type = 0,
        typename std::enable_if<std::is_arithmetic<typename naked_type<U>::type>::value, int>::type = 0>
    internal::ApproxEqCreator<T&&, U&&> ApproxEq(T &&expected, U &&margin) {
        internal::ApproxEqCreator<T&&, U&&> mc(std::forward<T>(expected), std::forward<U>(margin));
        return mc;
    }

}

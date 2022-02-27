/*
 * argument_matchers.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Jan 12, 2015
 */
#pragma once

#include <cstring>

namespace fakeit {

    struct IMatcher : Destructible {
        ~IMatcher() = default;
        virtual std::string format() const = 0;
    };

    template<typename T>
    struct TypedMatcher : IMatcher {
        virtual bool matches(const T &actual) const = 0;
    };

    template<typename T>
    struct TypedMatcherCreator {

        virtual ~TypedMatcherCreator() = default;

        virtual TypedMatcher<T> *createMatcher() const = 0;
    };

    template<typename T>
    struct ComparisonMatcherCreator : public TypedMatcherCreator<T> {

        virtual ~ComparisonMatcherCreator() = default;

        ComparisonMatcherCreator(const T &arg)
                : _expected(arg) {
        }

        struct Matcher : public TypedMatcher<T> {
            Matcher(const T &expected)
                    : _expected(expected) {
            }

            const T _expected;
        };

        const T &_expected;
    };

    namespace internal {
        template<typename T>
        struct TypedAnyMatcher : public TypedMatcherCreator<T> {

            virtual ~TypedAnyMatcher() = default;

            TypedAnyMatcher() {
            }

            struct Matcher : public TypedMatcher<T> {
                virtual bool matches(const T &) const override {
                    return true;
                }

                virtual std::string format() const override {
                    return "Any";
                }
            };

            virtual TypedMatcher<T> *createMatcher() const override {
                return new Matcher();
            }

        };

        template<typename T>
        struct EqMatcherCreator : public ComparisonMatcherCreator<T> {

            virtual ~EqMatcherCreator() = default;

            EqMatcherCreator(const T &expected)
                    : ComparisonMatcherCreator<T>(expected) {
            }

            struct Matcher : public ComparisonMatcherCreator<T>::Matcher {
                Matcher(const T &expected)
                        : ComparisonMatcherCreator<T>::Matcher(expected) {
                }

                virtual std::string format() const override {
                    return TypeFormatter<T>::format(this->_expected);
                }

                virtual bool matches(const T &actual) const override {
                    return actual == this->_expected;
                }
            };

            virtual TypedMatcher<T> *createMatcher() const {
                return new Matcher(this->_expected);
            }

        };

        template<typename T>
        struct GtMatcherCreator : public ComparisonMatcherCreator<T> {

            virtual ~GtMatcherCreator() = default;

            GtMatcherCreator(const T &expected)
                    : ComparisonMatcherCreator<T>(expected) {
            }

            struct Matcher : public ComparisonMatcherCreator<T>::Matcher {
                Matcher(const T &expected)
                        : ComparisonMatcherCreator<T>::Matcher(expected) {
                }

                virtual bool matches(const T &actual) const override {
                    return actual > this->_expected;
                }

                virtual std::string format() const override {
                    return std::string(">") + TypeFormatter<T>::format(this->_expected);
                }
            };

            virtual TypedMatcher<T> *createMatcher() const override {
                return new Matcher(this->_expected);
            }
        };

        template<typename T>
        struct GeMatcherCreator : public ComparisonMatcherCreator<T> {

            virtual ~GeMatcherCreator() = default;

            GeMatcherCreator(const T &expected)
                    : ComparisonMatcherCreator<T>(expected) {
            }

            struct Matcher : public ComparisonMatcherCreator<T>::Matcher {
                Matcher(const T &expected)
                        : ComparisonMatcherCreator<T>::Matcher(expected) {
                }

                virtual bool matches(const T &actual) const override {
                    return actual >= this->_expected;
                }

                virtual std::string format() const override {
                    return std::string(">=") + TypeFormatter<T>::format(this->_expected);
                }
            };

            virtual TypedMatcher<T> *createMatcher() const override {
                return new Matcher(this->_expected);
            }
        };

        template<typename T>
        struct LtMatcherCreator : public ComparisonMatcherCreator<T> {

            virtual ~LtMatcherCreator() = default;

            LtMatcherCreator(const T &expected)
                    : ComparisonMatcherCreator<T>(expected) {
            }

            struct Matcher : public ComparisonMatcherCreator<T>::Matcher {
                Matcher(const T &expected)
                        : ComparisonMatcherCreator<T>::Matcher(expected) {
                }

                virtual bool matches(const T &actual) const override {
                    return actual < this->_expected;
                }

                virtual std::string format() const override {
                    return std::string("<") + TypeFormatter<T>::format(this->_expected);
                }
            };

            virtual TypedMatcher<T> *createMatcher() const override {
                return new Matcher(this->_expected);
            }

        };

        template<typename T>
        struct LeMatcherCreator : public ComparisonMatcherCreator<T> {

            virtual ~LeMatcherCreator() = default;

            LeMatcherCreator(const T &expected)
                    : ComparisonMatcherCreator<T>(expected) {
            }

            struct Matcher : public ComparisonMatcherCreator<T>::Matcher {
                Matcher(const T &expected)
                        : ComparisonMatcherCreator<T>::Matcher(expected) {
                }

                virtual bool matches(const T &actual) const override {
                    return actual <= this->_expected;
                }

                virtual std::string format() const override {
                    return std::string("<=") + TypeFormatter<T>::format(this->_expected);
                }
            };

            virtual TypedMatcher<T> *createMatcher() const override {
                return new Matcher(this->_expected);
            }

        };

        template<typename T>
        struct NeMatcherCreator : public ComparisonMatcherCreator<T> {

            virtual ~NeMatcherCreator() = default;

            NeMatcherCreator(const T &expected)
                    : ComparisonMatcherCreator<T>(expected) {
            }

            struct Matcher : public ComparisonMatcherCreator<T>::Matcher {
                Matcher(const T &expected)
                        : ComparisonMatcherCreator<T>::Matcher(expected) {
                }

                virtual bool matches(const T &actual) const override {
                    return actual != this->_expected;
                }

                virtual std::string format() const override {
                    return std::string("!=") + TypeFormatter<T>::format(this->_expected);
                }

            };

            virtual TypedMatcher<T> *createMatcher() const override {
                return new Matcher(this->_expected);
            }

        };

        struct StrEqMatcherCreator : public ComparisonMatcherCreator<const char*> {

            virtual ~StrEqMatcherCreator() = default;

            StrEqMatcherCreator(const char* const &expected)
                    : ComparisonMatcherCreator<const char*>(expected) {
            }

            struct Matcher : public ComparisonMatcherCreator<const char*>::Matcher {
                Matcher(const char* const &expected)
                        : ComparisonMatcherCreator<const char*>::Matcher(expected) {
                }

                virtual std::string format() const override {
                    return TypeFormatter<const char*>::format(this->_expected);
                }

                virtual bool matches(const char* const &actual) const override {
                    return std::strcmp(actual, this->_expected) == 0;
                }
            };

            virtual TypedMatcher<const char*> *createMatcher() const {
                return new Matcher(this->_expected);
            }

        };

        struct StrGtMatcherCreator : public ComparisonMatcherCreator<const char*> {

            virtual ~StrGtMatcherCreator() = default;

            StrGtMatcherCreator(const char* const &expected)
                    : ComparisonMatcherCreator<const char*>(expected) {
            }

            struct Matcher : public ComparisonMatcherCreator<const char*>::Matcher {
                Matcher(const char* const &expected)
                        : ComparisonMatcherCreator<const char*>::Matcher(expected) {
                }

                virtual std::string format() const override {
                    return std::string(">") + TypeFormatter<const char*>::format(this->_expected);
                }

                virtual bool matches(const char* const &actual) const override {
                    return std::strcmp(actual, this->_expected) > 0;
                }
            };

            virtual TypedMatcher<const char*> *createMatcher() const {
                return new Matcher(this->_expected);
            }

        };

        struct StrGeMatcherCreator : public ComparisonMatcherCreator<const char*> {

            virtual ~StrGeMatcherCreator() = default;

            StrGeMatcherCreator(const char* const &expected)
                    : ComparisonMatcherCreator<const char*>(expected) {
            }

            struct Matcher : public ComparisonMatcherCreator<const char*>::Matcher {
                Matcher(const char* const &expected)
                        : ComparisonMatcherCreator<const char*>::Matcher(expected) {
                }

                virtual std::string format() const override {
                    return std::string(">=") + TypeFormatter<const char*>::format(this->_expected);
                }

                virtual bool matches(const char* const &actual) const override {
                    return std::strcmp(actual, this->_expected) >= 0;
                }
            };

            virtual TypedMatcher<const char*> *createMatcher() const {
                return new Matcher(this->_expected);
            }

        };

        struct StrLtMatcherCreator : public ComparisonMatcherCreator<const char*> {

            virtual ~StrLtMatcherCreator() = default;

            StrLtMatcherCreator(const char* const &expected)
                    : ComparisonMatcherCreator<const char*>(expected) {
            }

            struct Matcher : public ComparisonMatcherCreator<const char*>::Matcher {
                Matcher(const char* const &expected)
                        : ComparisonMatcherCreator<const char*>::Matcher(expected) {
                }

                virtual std::string format() const override {
                    return std::string("<") + TypeFormatter<const char*>::format(this->_expected);
                }

                virtual bool matches(const char* const &actual) const override {
                    return std::strcmp(actual, this->_expected) < 0;
                }
            };

            virtual TypedMatcher<const char*> *createMatcher() const {
                return new Matcher(this->_expected);
            }

        };

        struct StrLeMatcherCreator : public ComparisonMatcherCreator<const char*> {

            virtual ~StrLeMatcherCreator() = default;

            StrLeMatcherCreator(const char* const &expected)
                    : ComparisonMatcherCreator<const char*>(expected) {
            }

            struct Matcher : public ComparisonMatcherCreator<const char*>::Matcher {
                Matcher(const char* const &expected)
                        : ComparisonMatcherCreator<const char*>::Matcher(expected) {
                }

                virtual std::string format() const override {
                    return std::string("<=") + TypeFormatter<const char*>::format(this->_expected);
                }

                virtual bool matches(const char* const &actual) const override {
                    return std::strcmp(actual, this->_expected) <= 0;
                }
            };

            virtual TypedMatcher<const char*> *createMatcher() const {
                return new Matcher(this->_expected);
            }

        };

        struct StrNeMatcherCreator : public ComparisonMatcherCreator<const char*> {

            virtual ~StrNeMatcherCreator() = default;

            StrNeMatcherCreator(const char* const &expected)
                    : ComparisonMatcherCreator<const char*>(expected) {
            }

            struct Matcher : public ComparisonMatcherCreator<const char*>::Matcher {
                Matcher(const char* const &expected)
                        : ComparisonMatcherCreator<const char*>::Matcher(expected) {
                }

                virtual std::string format() const override {
                    return std::string("!=") + TypeFormatter<const char*>::format(this->_expected);
                }

                virtual bool matches(const char* const &actual) const override {
                    return std::strcmp(actual, this->_expected) != 0;
                }
            };

            virtual TypedMatcher<const char*> *createMatcher() const {
                return new Matcher(this->_expected);
            }

        };
    }

    struct AnyMatcher {
    } static _;

    template<typename T>
    internal::TypedAnyMatcher<T> Any() {
        internal::TypedAnyMatcher<T> rv;
        return rv;
    }

    template<typename T>
    internal::EqMatcherCreator<T> Eq(const T &arg) {
        internal::EqMatcherCreator<T> rv(arg);
        return rv;
    }

    template<typename T>
    internal::GtMatcherCreator<T> Gt(const T &arg) {
        internal::GtMatcherCreator<T> rv(arg);
        return rv;
    }

    template<typename T>
    internal::GeMatcherCreator<T> Ge(const T &arg) {
        internal::GeMatcherCreator<T> rv(arg);
        return rv;
    }

    template<typename T>
    internal::LtMatcherCreator<T> Lt(const T &arg) {
        internal::LtMatcherCreator<T> rv(arg);
        return rv;
    }

    template<typename T>
    internal::LeMatcherCreator<T> Le(const T &arg) {
        internal::LeMatcherCreator<T> rv(arg);
        return rv;
    }

    template<typename T>
    internal::NeMatcherCreator<T> Ne(const T &arg) {
        internal::NeMatcherCreator<T> rv(arg);
        return rv;
    }

    inline internal::StrEqMatcherCreator StrEq(const char* const &arg) {
        internal::StrEqMatcherCreator rv(arg);
        return rv;
    }

    inline internal::StrGtMatcherCreator StrGt(const char* const &arg) {
        internal::StrGtMatcherCreator rv(arg);
        return rv;
    }

    inline internal::StrGeMatcherCreator StrGe(const char* const &arg) {
        internal::StrGeMatcherCreator rv(arg);
        return rv;
    }

    inline internal::StrLtMatcherCreator StrLt(const char* const &arg) {
        internal::StrLtMatcherCreator rv(arg);
        return rv;
    }

    inline internal::StrLeMatcherCreator StrLe(const char* const &arg) {
        internal::StrLeMatcherCreator rv(arg);
        return rv;
    }

    inline internal::StrNeMatcherCreator StrNe(const char* const &arg) {
        internal::StrNeMatcherCreator rv(arg);
        return rv;
    }

}

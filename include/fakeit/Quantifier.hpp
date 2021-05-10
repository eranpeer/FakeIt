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

namespace fakeit {

    struct Quantity {
        Quantity(const int q) :
                quantity(q) {
        }

        const int quantity;
    } static Once(1);

    template<typename R>
    struct Quantifier : public Quantity {
        Quantifier(const int q, const R &val) :
                Quantity(q), value(val) {
        }

        const R &value;
    };

    template<>
    struct Quantifier<void> : public Quantity {
        explicit Quantifier(const int q) :
                Quantity(q) {
        }
    };

    struct QuantifierFunctor : public Quantifier<void> {
        QuantifierFunctor(const int q) :
                Quantifier<void>(q) {
        }

        template<typename R>
        Quantifier<R> operator()(const R &value) {
            return Quantifier<R>(quantity, value);
        }
    };

    template<int q>
    struct Times : public Quantity {

        Times() : Quantity(q) { }

        template<typename R>
        static Quantifier<R> of(const R &value) {
            return Quantifier<R>(q, value);
        }

        static Quantifier<void> Void() {
            return Quantifier<void>(q);
        }
    };

#if defined (__GNUG__) || (_MSC_VER >= 1900)

    inline QuantifierFunctor operator
    ""

    _Times(unsigned long long n) {
        return QuantifierFunctor((int) n);
    }

    inline QuantifierFunctor operator
    ""

    _Time(unsigned long long n) {
        if (n != 1)
            throw std::invalid_argument("Only 1_Time is supported. Use X_Times (with s) if X is bigger than 1");
        return QuantifierFunctor((int) n);
    }

#endif

}

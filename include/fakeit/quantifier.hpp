/*
 * quantifier.hpp
 *
 *  Created on: Mar 10, 2014
 *      Author: eran
 */

#ifndef QUANTIFIER_HPP_
#define QUANTIFIER_HPP_

#include <functional>
#include <type_traits>
#include "mockutils/DefaultValue.hpp"

namespace fakeit {

template<typename R>
struct Quantifier {
	Quantifier(const int quantity, const R& value) :
			quantity(quantity), value(value) {
	}

	const int quantity;
	const R& value;

	Quantifier<R> & operator()(const R& value) {
		this->value = value;
		return *this;
	}
};

template<>
struct Quantifier<void> {
	Quantifier(const int quantity) :
			quantity(quantity) {
	}
	const int quantity;
};

struct QuantifierFunctor: public Quantifier<void> {
	QuantifierFunctor(const int quantity) :
			Quantifier<void>(quantity) {
	}

	template<typename R>
	Quantifier<R> operator()(const R& value) {
		return Quantifier<R>(quantity, value);
	}
};

template<int q>
struct Times {

	template<typename R>
	static Quantifier<R> of(const R& value) {
		return Quantifier<R>(q, value);
	}

	static Quantifier<void> Void() {
		return Quantifier<void>(q);
	}

};

#if defined (__GNUG__)
// Only supported by GCC

inline QuantifierFunctor operator"" _Times(unsigned long long n)
{
	return QuantifierFunctor((int)n);
}

#endif
}


#endif /* QUANTIFIER_HPP_ */

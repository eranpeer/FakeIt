/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef TuplePrinter_h__
#define TuplePrinter_h__

#include <iostream>
#include <tuple>
#include <string>
#include <sstream>
#include "mockutils/Formatter.hpp"

namespace fakeit {

// helper function to print a tuple of any size
template<class Tuple, std::size_t N>
struct TuplePrinter {
	static void print(std::ostream &strm, const Tuple& t) {
		TuplePrinter<Tuple, N - 1>::print(strm, t);
		strm << ", " << fakeit::Formatter<decltype(std::get<N - 1>(t))>::format(std::get < N - 1 > (t));
	}
};

template<class Tuple>
struct TuplePrinter<Tuple, 1> {
	static void print(std::ostream &strm, const Tuple& t) {
		strm << fakeit::Formatter<decltype(std::get<0>(t))>::format(std::get < 0 > (t));
	}
};

template<class Tuple>
struct TuplePrinter<Tuple, 0> {
    static void print(std::ostream &, const Tuple& ) {
	}
};

template<class ... Args>
void print(std::ostream &strm, const std::tuple<Args...>& t) {
	strm << "(";
	TuplePrinter<decltype(t), sizeof...(Args)>::print(strm, t);
	strm << ")";
}

template<class ... Args>
std::ostream & operator<<(std::ostream &strm, const std::tuple<Args...>& t) {
	print(strm, t);
	return strm;
}

}
#endif // TuplePrinter_h__

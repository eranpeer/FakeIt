/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef TupleDispatcher_h__
#define TupleDispatcher_h__
#include <tuple>

namespace fakeit {

template<int N>
struct apply_func {
	template<typename ... ArgsF, typename ... ArgsT, typename ... Args>
	static bool applyTuple(std::function<bool (ArgsF...)> f, const std::tuple<ArgsT...>& t, Args&... args) {
		return apply_func<N - 1>::applyTuple(f, t, std::get < N - 1 > (t), args...);
	}
};

template<>
struct apply_func<0> {
	template<typename ... ArgsF, typename ... ArgsT, typename ... Args>
	static bool applyTuple(std::function<bool (ArgsF...)> f, const std::tuple<ArgsT...>& /* t */, Args&... args) {
		return f(args...);
	}
};

template<typename ... ArgsF, typename ... ArgsT>
bool applyTuple(std::function<bool (ArgsF...)> f, std::tuple<ArgsT...> const& t) {
	return apply_func<sizeof...(ArgsT)>::applyTuple(f, t);
}

template<typename ...arglist>
bool invoke(std::function<bool(arglist...)> func, const std::tuple<arglist...>& arguments) {
	return applyTuple(func, arguments);
}

template<int N>
struct verifyTypes {
	template<typename Head, typename ... ArgsT, typename ... Tail>
	static bool applyTuple(const std::tuple<ArgsT...>& t, Head &h, Tail&... tail) {
		//h should be comparable from std::get<N - sizeof...(Tail)  - 1>(t);
		//static_assert(???, "not assignable");
		return verifyTypes<sizeof...(Tail)>::applyTuple(t, tail...);
	}
};

template<>
struct verifyTypes<0> {
	template<typename ... ArgsT>
	static bool applyTuple(const std::tuple<ArgsT...>& /* t */
	) {
		return true;
	}
};

template<typename ... ArgsT, typename ... Args>
bool checkTypes(std::tuple<ArgsT...> const& t, Args ... args) {
	static_assert(sizeof...(ArgsT) == sizeof...(Args), "argument lists are not the same size");
	return verifyTypes<sizeof...(ArgsT)>::applyTuple(t, args...);
}

}

#endif // TupleDispatcher_h__

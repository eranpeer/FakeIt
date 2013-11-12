#ifndef TupleDispatcher_h__
#define TupleDispatcher_h__
#include <tuple>

template < int N >
struct apply_func
{
	template < typename... ArgsF, typename... ArgsT, typename... Args >
	static bool applyTuple(std::function<bool((ArgsF...))> f,
		const std::tuple<ArgsT...>& t,
		Args... args)
	{
		return apply_func<N - 1>::applyTuple(f, t, std::get<N - 1>(t), args...);
	}
};

template <>
struct apply_func<0>
{
	template < typename... ArgsF, typename... ArgsT, typename... Args >
	static bool applyTuple(std::function<bool((ArgsF...))> f,
		const std::tuple<ArgsT...>& /* t */,
		Args... args)
	{
		return f(args...);
	}
};

template < typename... ArgsF, typename... ArgsT >
bool applyTuple(std::function<bool((ArgsF...))> f,
	std::tuple<ArgsT...> const& t)
{
	return apply_func<sizeof...(ArgsT)>::applyTuple(f, t);
}

template <typename ...arglist>
bool filter(std::function<bool(arglist...)> func, const std::tuple<arglist...>& params ){
	return applyTuple(func, params);
}

#endif // TupleDispatcher_h__
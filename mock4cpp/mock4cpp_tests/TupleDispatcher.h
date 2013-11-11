
#ifndef TupleDispatcher_h__
#define TupleDispatcher_h__

// 
// template <typename ...arglist>
// struct TupleDispatcher {
// 	
// 	bool dispatch(std::function<bool(arglist...)> func, arglist ...args) {
// 		return func(args...);
// 	}
// 
// 	template <typename ...Actual>
// 	typename std::enable_if<sizeof...(Actual) != sizeof...(arglist), bool>::type
// 		dispatch(std::function<bool(arglist...)> func, Actual&& ...actual) {
// 			return dispatch(func, std::forward<Actual>(actual)..., std::get<sizeof...(Actual)>(params));
// 		}
// 
// 	TupleDispatcher(const std::tuple<arglist...>& params) :params{ params }{}
// 
// private:
// 	std::tuple<arglist...> params;
// };


#include <tuple>

template <int N>
struct apply_obj_func
{
	template < typename T, typename... ArgsF, typename... ArgsT, typename... Args >
	static void applyTuple(T* pObj,
		void(T::*f)(ArgsF...),
		const std::tr1::tuple<ArgsT...>& t,
		Args... args)
	{
		apply_obj_func<N - 1>::applyTuple(pObj, f, t, std::tr1::get<N - 1>(t), args...);
	}
};

//-----------------------------------------------------------------------------

template <>
struct apply_obj_func<0>
{
	template < typename T, typename... ArgsF, typename... ArgsT, typename... Args >
	static void applyTuple(T* pObj,
		void(T::*f)(ArgsF...),
		const std::tr1::tuple<ArgsT...>& /* t */,
		Args... args)
	{
		(pObj->*f)(args...);
	}
};

template < typename T, typename... ArgsF, typename... ArgsT >
void applyTuple(T* pObj,
	void(T::*f)(ArgsF...),
	std::tr1::tuple<ArgsT...> const& t)
{
	apply_obj_func<sizeof...(ArgsT)>::applyTuple(pObj, f, t);
}

//-----------------------------------------------------------------------------

template < int N >
struct apply_func
{
	template < typename... ArgsF, typename... ArgsT, typename... Args >
	static bool applyTuple(std::function<bool((ArgsF...))> f,
		const std::tr1::tuple<ArgsT...>& t,
		Args... args)
	{
		return apply_func<N - 1>::applyTuple(f, t, std::tr1::get<N - 1>(t), args...);
	}
};

template <>
struct apply_func<0>
{
	template < typename... ArgsF, typename... ArgsT, typename... Args >
	static bool applyTuple(std::function<bool((ArgsF...))> f,
		const std::tr1::tuple<ArgsT...>& /* t */,
		Args... args)
	{
		return f(args...);
	}
};

template < typename... ArgsF, typename... ArgsT >
bool applyTuple(std::function<bool((ArgsF...))> f,
	std::tr1::tuple<ArgsT...> const& t)
{
	return apply_func<sizeof...(ArgsT)>::applyTuple(f, t);
}




template <typename ...arglist>
bool filter(std::function<bool(arglist...)> func, std::tuple<arglist...>& params ){
	return applyTuple(func, params);
}

#endif // TupleDispatcher_h__


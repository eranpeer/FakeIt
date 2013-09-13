#ifndef DefaultValue_h__
#define DefaultValue_h__
#include <type_traits>

struct DefaultValue {


	template<typename F, class REF = std::add_reference<F>::type>
	static typename std::enable_if<
		!std::is_reference<F>::value &&
		std::is_scalar<F>::value
		, REF>::type value()
	{
		static F val{};
		return val;
	}

	template<typename C, class REF = std::add_reference<C>::type>
	static typename 
		std::enable_if<
		!std::is_reference<C>::value &&
		std::is_class<C>::value && 
		!std::is_abstract<C>::value && 	
		!std::is_default_constructible<C>::value
		, REF>::type 
	value()
	{
		throw "could not instantiate default value for class";
	}

	template<typename C, class REF = std::add_reference<C>::type>
	static typename 
		std::enable_if<
		!std::is_reference<C>::value &&
		std::is_class<C>::value && 
		!std::is_abstract<C>::value && 
		std::is_default_constructible<C>::value
		, REF>::type 
	value()
	{
		static C val{};
		return val;
	}

	template<typename AC, class REF = std::add_reference<AC>::type>
	static typename std::enable_if<std::is_class<AC>::value && std::is_abstract<AC>::value && !std::is_reference<AC>::value, REF>::type value()
	{
		AC* ptrToNull{ nullptr };
		return *ptrToNull;
	}

	template<typename REF, class T = std::remove_reference<REF>::type>
	static typename std::enable_if<std::is_reference<REF>::value, REF>::type value()
	{
		return DefaultValue::value<T>();
	}

};

#endif // DefaultValue_h__



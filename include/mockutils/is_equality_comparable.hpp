#ifndef is_equality_comparable_h__
#define is_equality_comparable_h__

#include <type_traits>

template<typename T, typename = void>
struct is_equality_comparable : std::false_type
{ };

template<typename T>
struct is_equality_comparable < T,
	typename std::enable_if <
	true,
	decltype((std::declval<T>() == std::declval<T>()), (void) 0)
	> ::type
> : std::true_type
{
};

#endif // is_equality_comparable_h__


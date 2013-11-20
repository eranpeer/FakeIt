#ifndef are_equality_comparable_h__
#define are_equality_comparable_h__


#include <type_traits>

template<typename LEFT, typename RIGHT, typename = void>
struct are_equality_comparable : std::false_type
{ };

template<typename LEFT, typename RIGHT>
struct are_equality_comparable<
	LEFT,
	RIGHT,
	typename std::enable_if<
	true,
	decltype(std::declval<LEFT>() == std::declval<RIGHT>(), (void) 0)
	>::type
> : std::true_type
{
};

#endif // are_equality_comparable_h__

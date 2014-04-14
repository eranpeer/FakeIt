/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef traits_h__
#define traits_h__

namespace fakeit {

#include <type_traits>
template<class T>
struct is_copy_initializable: std::integral_constant<bool,
		(std::is_copy_constructible < T > ::value && !std::is_abstract < T > ::value)
				|| std::is_same<std::nullptr_t, typename std::remove_cv<T>::type>::value> {
};

template<class T>
struct is_default_initializable: std::integral_constant<bool,
		(std::is_default_constructible < T > ::value && !std::is_abstract < T > ::value)
				|| std::is_same<std::nullptr_t, typename std::remove_cv<T>::type>::value> {
};

}
#endif // traits_h__

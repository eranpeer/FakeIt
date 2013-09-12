#ifndef DefaultValue_h__
#define DefaultValue_h__

#include <type_traits>
template < typename T, class U = std::remove_reference<std::remove_cv<T>::type>::type>
struct DefaultValue {

	static T value(){
		static U val{};
		return val;
	}

};
#endif // DefaultValue_h__



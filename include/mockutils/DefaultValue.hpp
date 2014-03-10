#ifndef DefaultValue_h__
#define DefaultValue_h__
#include <type_traits>
#include <typeinfo>

namespace fakeit {

struct DefaultValueInstatiationException: public std::exception {
	DefaultValueInstatiationException() {
	}

	const char* what() const throw () override {
		return std::string("could not instantiate default value for type").c_str();
	}
};

struct DefaultValue {

	// Type is default constructible => It can have a default value.
	template<typename C>
	static typename std::enable_if<std::is_void<C>::value, C>::type value() {
	}

	// Type is default constructible => It can have a default value.
	//template<typename C, class REF = std::add_reference<C>::type>
	template<typename C>
	static typename std::enable_if<
			!std::is_void<C>::value && !std::is_reference<C>::value && !std::is_abstract<C>::value
					&& std::is_default_constructible<C>::value, C&>::type value() {
		static C val { };
		return val;
	}

	// Can't create default value of an a non default constructible type.
	template<typename C>
	static typename std::enable_if<
			!std::is_void<C>::value && !std::is_reference<C>::value && !std::is_abstract<C>::value
					&& !std::is_default_constructible<C>::value, C&>::type value() {
		throw DefaultValueInstatiationException();
//		throw std::string(typeid(C).name())
//				+ std::string(" does not have a default constructor. could not instantiate default value for type");
	}

	// Can't create default value of an abstract type
	template<typename AC>
	static typename std::enable_if<!std::is_reference<AC>::value && std::is_abstract<AC>::value, AC&>::type value() {
		throw DefaultValueInstatiationException();
		//std::string(typeid(AC).name()) + std::string(" is abstract. could not instantiate default value for class");
	}

	template<typename REF>
	static typename std::enable_if<std::is_reference<REF>::value, REF>::type value() {
		bool isAbstract = std::is_abstract<typename std::remove_reference<REF>::type>::value;
		if (isAbstract) {
			typename std::remove_reference<REF>::type * ptr = nullptr;
			return *ptr;
		}
		return DefaultValue::value<typename std::remove_reference<REF>::type>();
	}

};

}
#endif // DefaultValue_h__

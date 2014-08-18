/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef DefaultValue_h__
#define DefaultValue_h__
#include <type_traits>
#include <typeinfo>

namespace fakeit {

struct DefaultValueInstatiationException {
	DefaultValueInstatiationException() = default;
	virtual std::string what() const = 0;
};

struct DefaultValue {

	template<typename C>
	static typename std::enable_if<std::is_void<C>::value, C>::type value() {
	}

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
			!std::is_void<C>::value && !std::is_reference<C>::value
					&& !std::is_default_constructible<C>::value, C&>::type value() {

		class Exception : public DefaultValueInstatiationException {
			virtual std::string what() const override {
				return (std::string("Type ") + std::string(typeid(C).name())
				+ std::string(" is not default constructible. Could not instantiate a default return value")).c_str();
			}
		};

		throw Exception();
	}

	template<typename REF>
	static typename std::enable_if<
		std::is_reference<REF>::value && 
		std::is_default_constructible<typename std::remove_reference<REF>::type>::value &&
		!std::is_abstract<typename std::remove_reference<REF>::type>::value,
		REF>::type value() {
		return DefaultValue::value<typename std::remove_reference<REF>::type>();
	}

	template<typename REF>
	static typename std::enable_if<
		std::is_reference<REF>::value
		&& (!std::is_default_constructible<typename std::remove_reference<REF>::type>::value
		|| std::is_abstract<typename std::remove_reference<REF>::type>::value),
		REF>::type value() {
			typename std::remove_reference<REF>::type * ptr = nullptr;
			return *ptr;
		}
};

}
#endif // DefaultValue_h__

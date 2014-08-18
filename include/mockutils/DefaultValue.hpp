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

//struct DefaultValue {
//
//	template<typename C>
//	static typename std::enable_if<std::is_void<C>::value, C>::type value() {
//	}
//
//	template<typename C>
//	static typename std::enable_if<
//		!std::is_void<C>::value && !std::is_reference<C>::value && !std::is_abstract<C>::value
//					&& std::is_default_constructible<C>::value, C&>::type value() {
//		static C val { };
//		return val;
//	}
//
//	// Can't create default value of an a non default constructible type.
//	template<typename C>
//	static typename std::enable_if<
//			!std::is_void<C>::value && !std::is_reference<C>::value
//					&& !std::is_default_constructible<C>::value, C&>::type value() {
//
//		class Exception : public DefaultValueInstatiationException {
//			virtual std::string what() const override {
//				return (std::string("Type ") + std::string(typeid(C).name())
//				+ std::string(" is not default constructible. Could not instantiate a default return value")).c_str();
//			}
//		};
//
//		throw Exception();
//	}
//
//	template<typename REF>
//	static typename std::enable_if<
//		std::is_reference<REF>::value &&
//		std::is_default_constructible<typename std::remove_reference<REF>::type>::value &&
//		!std::is_abstract<typename std::remove_reference<REF>::type>::value,
//		REF>::type value() {
//		return DefaultValue::value<typename std::remove_reference<REF>::type>();
//	}
//
//	template<typename REF>
//	static typename std::enable_if<
//		std::is_reference<REF>::value
//		&& (!std::is_default_constructible<typename std::remove_reference<REF>::type>::value
//		|| std::is_abstract<typename std::remove_reference<REF>::type>::value),
//		REF>::type value() {
//			typename std::remove_reference<REF>::type * ptr = nullptr;
//			return *ptr;
//		}
//};


//template<class C>
//struct AbstractTypeDefaultValue {
//	// this was a reference to abstract type
//	static C& value() {
//		C* ptr = nullptr;
//		return *ptr;
//	}
//};

template<class C, class Enable = void>
struct ConstructableTypeDefaultValue;


template<class C>
struct ConstructableTypeDefaultValue <C, typename std::enable_if<!std::is_default_constructible<C>::value>::type> {
	// this was a reference to abstract type
	static C& value() {
		throw 0;
	}
};

template<class C>
struct ConstructableTypeDefaultValue <C, typename std::enable_if<std::is_default_constructible<C>::value>::type> {
	// this was a reference to abstract type
	static C& value() {
		static C val { };
		return val;
	}
};

template<class C>
struct DefaultValue2 {
	static C& value() {

		if (std::is_default_constructible<typename std::remove_reference<C>::type>::value)
			return ConstructableTypeDefaultValue<typename std::remove_reference<C>::type>::value();

		if (std::is_reference<C>::value){
			typename std::remove_reference<C>::type * ptr = nullptr;
			return *ptr;
		}

		class Exception : public DefaultValueInstatiationException {
			virtual std::string what() const override {
				return (std::string("Type ") + std::string(typeid(C).name())
				+ std::string(" is not default constructible. Could not instantiate a default return value")).c_str();
			}
		};

		throw Exception();

	}
};


template<> struct DefaultValue2<void> {
	static void value() {
		return;
	}
};

template<> struct DefaultValue2<bool> {
	static bool& value() {
		static bool value { false };
		return value;
	}
};

template<> struct DefaultValue2<char> {
	static char& value() {
		static char value { 0 };
		return value;
	}
};

template<> struct DefaultValue2<char16_t> {
	static char16_t& value() {
		static char16_t value { 0 };
		return value;
	}
};
template<> struct DefaultValue2<char32_t> {
	static char32_t& value() {
		static char32_t value { 0 };
		return value;
	}
};
template<> struct DefaultValue2<wchar_t> {
	static wchar_t& value() {
		static wchar_t value { 0 };
		return value;
	}
};
template<> struct DefaultValue2<short> {
	static short& value() {
		static short value { 0 };
		return value;
	}
};
template<> struct DefaultValue2<int> {
	static int& value() {
		static int value { 0 };
		return value;
	}
};
template<> struct DefaultValue2<long> {
	static long& value() {
		static long value { 0 };
		return value;
	}
};
template<> struct DefaultValue2<long long> {
	static long long& value() {
		static long long value { 0 };
		return value;
	}
};

}
#endif // DefaultValue_h__

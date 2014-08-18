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
	virtual ~DefaultValueInstatiationException() = default;
	DefaultValueInstatiationException() = default;
	virtual std::string what() const = 0;
};

template<class C, class Enable = void>
struct DefaultValue;


template<class C>
struct DefaultValue <C, typename std::enable_if<!std::is_default_constructible<typename std::remove_reference<C>::type>::value>::type> {
	// this was a reference to abstract type
	static C& value() {
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

template<class C>
struct DefaultValue <C, typename std::enable_if<std::is_default_constructible<typename std::remove_reference<C>::type>::value>::type> {
	// this was a reference to abstract type
	static C& value() {
		static typename std::remove_reference<C>::type val { };
		return val;
	}
};


template<> struct DefaultValue<void> {
	static void value() {
		return;
	}
};

template<> struct DefaultValue<bool> {
	static bool& value() {
		static bool value { false };
		return value;
	}
};

template<> struct DefaultValue<char> {
	static char& value() {
		static char value { 0 };
		return value;
	}
};

template<> struct DefaultValue<char16_t> {
	static char16_t& value() {
		static char16_t value { 0 };
		return value;
	}
};
template<> struct DefaultValue<char32_t> {
	static char32_t& value() {
		static char32_t value { 0 };
		return value;
	}
};
template<> struct DefaultValue<wchar_t> {
	static wchar_t& value() {
		static wchar_t value { 0 };
		return value;
	}
};
template<> struct DefaultValue<short> {
	static short& value() {
		static short value { 0 };
		return value;
	}
};
template<> struct DefaultValue<int> {
	static int& value() {
		static int value { 0 };
		return value;
	}
};
template<> struct DefaultValue<long> {
	static long& value() {
		static long value { 0 };
		return value;
	}
};
template<> struct DefaultValue<long long> {
	static long long& value() {
		static long long value { 0 };
		return value;
	}
};

}
#endif // DefaultValue_h__

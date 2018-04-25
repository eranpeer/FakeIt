/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include <ostream>
#include "mockutils/type_utils.hpp"

namespace fakeit {

	template<typename T, class Enable = void>
	struct Formatter;

	template <>
	struct Formatter<bool>
	{
		static std::string format(bool const &val)
		{
			return val ? "true" : "false";
		}
	};

	template <>
	struct Formatter<char>
	{
		static std::string format(char const &val)
		{
			std::string s;
			s += "'";
			s += val;
			s += "'";
			return s;
		}
	};

	template <>
	struct Formatter<char const*>
	{
		static std::string format(char const* const &val)
		{
			std::string s;
			if(val != nullptr)
			{
				s += '"';
				s += val;
				s += '"';
			}
			else
			{
				s = "[nullptr]";
			}
			return s;
		}
	};

	template <>
	struct Formatter<char*>
	{
		static std::string format(char* const &val)
		{
			return Formatter<char const*>::format( val );
		}
	};

	template<class C>
	struct Formatter<C, typename std::enable_if<!is_ostreamable<C>::value>::type> {
		static std::string format(C const &)
		{
			return "?";
		}
	};

	template<class C>
	struct Formatter<C, typename std::enable_if<is_ostreamable<C>::value>::type> {
		static std::string format(C const &val)
		{
			std::ostringstream os;
			os << val;
			return os.str();
		}
	};


	template <typename T>
	using TypeFormatter = Formatter<typename fakeit::naked_type<T>::type>;
}
/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef Invocation_h__
#define Invocation_h__

#include <typeinfo>
#include <unordered_set>
#include <tuple>
#include <string>
#include <iostream>
#include <sstream>

#include "fakeit/DomainObjects.hpp"

namespace fakeit {

struct Invocation {

	Invocation(const int ordinal, const Method & method) :
			ordinal(ordinal), method(method), _isVerified(false) {
	}

	virtual ~Invocation() = default;

	int getOrdinal() const {
		return ordinal;
	}

	const Method & getMethod() const {
		return method;
	}

	void markAsVerified() {
		_isVerified = true;
	}

	bool isVerified() const {
		return _isVerified;
	}

	virtual std::string format() const = 0;

	struct Matcher {
		virtual ~Matcher() THROWS {
		}
		virtual bool matches(Invocation& invocation) = 0;

		virtual std::string format() const = 0;
	};

private:
	const int ordinal;
	const Method & method;
	bool _isVerified;
};

}

#endif // Invocation_h__

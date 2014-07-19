/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef MethodVerificationProgress_h__
#define MethodVerificationProgress_h__

#include <functional>
#include <type_traits>
#include <unordered_set>
#include <vector>
#include <stdexcept>
#include <utility>

#include "fakeit/quantifier.hpp"
#include "mockutils/Macros.hpp"

namespace fakeit {

struct MethodVerificationProgress {

	MethodVerificationProgress() {
	}

	virtual ~MethodVerificationProgress() THROWS {
	}

	void Never() {
		Exactly(0);
	}

	virtual void Once() {
		Exactly(1);
	}

	virtual void Twice() {
		Exactly(2);
	}

	virtual void AtLeastOnce() {
		verifyInvocations(-1);
	}

	virtual void Exactly(const int times) {
		if (times < 0) {
			throw std::invalid_argument(std::string("bad argument times:").append(std::to_string(times)));
		}
		verifyInvocations(times);
	}

	virtual void Exactly(const Quantity & q) {
		Exactly(q.quantity);
	}

	virtual void AtLeast(const int times) {
		if (times < 0) {
			throw std::invalid_argument(std::string("bad argument times:").append(std::to_string(times)));
		}
		verifyInvocations(-times);
	}

	virtual void AtLeast(const Quantity & q) {
		AtLeast(q.quantity);
	}

protected:
	virtual void verifyInvocations(const int times) = 0;
private:
	MethodVerificationProgress & operator=(const MethodVerificationProgress & other) = delete;
};

}

#endif // MethodVerificationProgress_h__

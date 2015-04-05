/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include <string>
#include <iosfwd>
#include <stdexcept>
#include <tuple>

#include "tpunit++.hpp"
#include "fakeit.hpp"
#include "mockutils/Formatter.hpp"

struct A {
	int state;
	A() :
			state(0) {
	}

	bool operator==(const A& other) const {
		return other.state == state;
	}
};

namespace fakeit {
template<> struct Formatter<A> {
	static std::string format(const A&) {
		return {"a"};
	}
};

}

using namespace fakeit;

struct ErrorFormattingTests: tpunit::TestFixture {

	struct SomeInterface {
		virtual void foo(int) = 0;
	};

	ErrorFormattingTests() :
			tpunit::TestFixture(
					//
					TEST(ErrorFormattingTests::check_atleast_verification_exception),
					TEST(ErrorFormattingTests::check_exact_verification_exception),
					TEST(ErrorFormattingTests::check_no_more_invocations_exception)) //
	{
	}

	template <typename T> std::string to_string(T& val){
		std::stringstream stream;
		stream << val;
		return stream.str();
	}

	void check_atleast_verification_exception() {
		Mock<SomeInterface> mock;
		try {
			auto selector = Method(mock,foo);
			Verify(selector);
		} catch (SequenceVerificationException& ) {
		}
	}

	void check_exact_verification_exception() {
		Mock<SomeInterface> mock;
		try {
			auto selector = Method(mock,foo);
			Verify(selector).Exactly(Once);
		} catch (SequenceVerificationException& ) {
		}
	}

	void check_no_more_invocations_exception() {
		Mock<SomeInterface> mock;
		try {
			Fake(Method(mock,foo));
			mock.get().foo(1);
			mock.get().foo(2);
			Verify(Method(mock,foo).Using(1));
			VerifyNoOtherInvocations(Method(mock,foo));
		} catch (NoMoreInvocationsVerificationException&) {
//			ASSERT_EQUAL(VerificationType::NoMoreInvocations, e.verificationType());
//			ASSERT_EQUAL(2, e.allIvocations().size());
//			ASSERT_EQUAL(1, e.unverifedIvocations().size());
		}
	}

} __ErrorFormatting;

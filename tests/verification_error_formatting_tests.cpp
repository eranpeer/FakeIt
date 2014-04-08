#include <string>
#include <iostream>
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
	static std::string format(const A& val) {
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
			tpunit::TestFixture(TEST(ErrorFormattingTests::check_atleast_exception),
					TEST(ErrorFormattingTests::check_no_more_invocations_exception)
			) //
	{
	}

	void check_atleast_exception() {
		Mock<SomeInterface> mock;
		try {
			auto selector = mock[&ErrorFormattingTests::SomeInterface::foo];
			Verify(selector);
		} catch (SequenceVerificationException& e) {
			ASSERT_EQUAL(VerificationType::AtLeast, e.verificationType());
			ASSERT_EQUAL(0, e.actualCount());
			ASSERT_EQUAL(1, e.expectedCount());
			ASSERT_EQUAL(0, e.actualSequence().size());
			ASSERT_EQUAL(1, e.expectedPattern().size());
//			std::string expected("Expected invocation sequence could not be found in actual invocation order");
//			std::string actual(e.what());
//			ASSERT_EQUAL(expected, actual);
		}
	}

	void check_no_more_invocations_exception() {
		Mock<SomeInterface> mock;
		try {
			Fake(mock[&ErrorFormattingTests::SomeInterface::foo]);
			mock.get().foo(1);
			mock.get().foo(2);
			Verify(mock[&SomeInterface::foo].Using(1));
			VerifyNoOtherInvocations(mock[&SomeInterface::foo]);
		} catch (NoMoreInvocationsVerificationException& e) {
			ASSERT_EQUAL(VerificationType::NoMoreInvocatoins, e.verificationType());
			ASSERT_EQUAL(2, e.allIvocations().size());
			ASSERT_EQUAL(1, e.unverifedIvocations().size());
		}
	}

} __ErrorFormatting;

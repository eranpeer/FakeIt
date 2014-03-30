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
		virtual void foo(bool b, char c, short s, int i, long l, double d) = 0;
	};

	ErrorFormattingTests() :
			tpunit::TestFixture(TEST(ErrorFormattingTests::format_scalars)) //
	{
	}

	void format_scalars() {
		Mock<SomeInterface> mock;
		try {
			Verify(mock[&ErrorFormattingTests::SomeInterface::foo]);
		} catch (VerificationException& e) {
			std::string expected("Expected invocation scenario does not match actual invocation order");
			std::string actual(e.what());
			ASSERT_EQUAL(expected, actual);
		}
	}

} __ErrorFormatting;

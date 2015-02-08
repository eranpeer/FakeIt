/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#if defined (__GNUG__)

#include <string>
#include <iosfwd>
#include <stdexcept>
#include <tuple>
#include <type_traits>

#include "tpunit++.hpp"
#include "fakeit.hpp"
#include "mockutils/Formatter.hpp"

using namespace fakeit;

struct GccTypeInfoTests: tpunit::TestFixture {

	GccTypeInfoTests() :
			tpunit::TestFixture( //
					TEST(GccTypeInfoTests::simple_inheritance_dynamic_down_cast), TEST(GccTypeInfoTests::mutiple_inheritance_upcast)) {
	}

	struct TopLeft {
		int topLeft;
		virtual int f()=0;
	};

	struct Left: public TopLeft {
		int left;
		virtual int f() override = 0;
	};

	struct A: public Left {
		int a;
		virtual int f() override = 0;
	};

	void simple_inheritance_dynamic_down_cast() {
		Mock<A> aMock; // no need for base classes list on gcc.
		Fake(Method(aMock,f));
		A& a = aMock.get();
		Left* left = &a;
		TopLeft* topLeft = &a;

		A* aPtr = dynamic_cast<A*>(left);
		ASSERT_EQUAL(0, aPtr->f());

		aPtr = dynamic_cast<A*>(topLeft);
		ASSERT_EQUAL(0, aPtr->f());

		left = dynamic_cast<Left*>(topLeft);
		ASSERT_EQUAL(0, left->f());
	}

	struct TopRight {
		int topRight;
		virtual int f()=0;
	};

	struct Right: public TopRight {
		int right;
		virtual int f() override = 0;
	};

	struct B: public Left, public Right {
		int b;
		virtual int f() override = 0;
	};

	void mutiple_inheritance_upcast() {
		//Mock<B> bMock; // should not compile
	}

} __GccTypeInfoTests;

#endif

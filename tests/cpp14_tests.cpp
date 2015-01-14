/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#if __cplusplus > 201103L

#include <string>
#include "tpunit++.hpp"
#include "fakeit.hpp"

using namespace fakeit;

struct Cpp14Tests : tpunit::TestFixture {
	Cpp14Tests() :
	tpunit::TestFixture(
			//
			TEST(Cpp14Tests::use_cpp14_lambda_in_do),
			TEST(Cpp14Tests::use_cpp14_lambda_in_invocation_matching),
			TEST(Cpp14Tests::use_cpp14_lambda_in_verifing)
	)//
	{
	}

	struct SomeInterface {
		virtual int foo(int a, std::string bar) = 0;
		virtual void bar(int a, std::string &bar) = 0;
	};

	void use_cpp14_lambda_in_do() {
		Mock<SomeInterface> mock;
		When(Method(mock,foo)).Do([](auto a, auto b) {return a;});
		ASSERT_EQUAL(1, mock().foo(1,""));
		When(Method(mock,foo)).Do([](auto& a, auto& b) {return a;});
		ASSERT_EQUAL(2, mock().foo(2,""));
	}

	void use_cpp14_lambda_in_invocation_matching() {
		Mock<SomeInterface> mock;
		When(Method(mock,foo).Matching([](auto a, auto b) {return b == "A";})).Return(1);
		When(Method(mock,foo).Matching([](auto &a, auto &b) {return b == "B";})).Return(2);
		When(Method(mock,foo).Matching([](auto const a, auto const & b) {return b == "C";})).Return(3);

		ASSERT_EQUAL(1, mock().foo(0,"A"));
		ASSERT_EQUAL(2, mock().foo(0,"B"));
		ASSERT_EQUAL(3, mock().foo(0,"C"));
	}

	void use_cpp14_lambda_in_verifing() {
		Mock<SomeInterface> mock;
		When(Method(mock,foo)).AlwaysReturn(1);

		mock().foo(1,"A");
		mock().foo(2,"B");

		Verify(Method(mock,foo).Matching([](auto a, auto b) {return b == "A";}));
		Verify(Method(mock,foo).Matching([](auto &a, auto &b) {return b == "A";}));
		Verify(Method(mock,foo).Matching([](const auto a, const auto b) {return b == "A";}));

		ASSERT_THROW(
				Verify(
						Method(mock,foo).Matching(
								[](auto a, auto b) {
									return b == "C";
								}
						)
				), fakeit::VerificationException);
	}

}__Cpp14Tests;

#endif

/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include <string>
#include <queue>

#include "tpunit++.hpp"
#include "fakeit.hpp"

using namespace fakeit;

struct MultipleStubbing: tpunit::TestFixture {
	MultipleStubbing() :
			tpunit::TestFixture(
					//
					TEST(MultipleStubbing::stub_multiple_do_with_list_of_mixed_quatifiers_and_values),
					TEST(MultipleStubbing::stub_multiple_throws_with_quantifier),
					TEST(MultipleStubbing::stub_multiple_return_values_with_mixed_values_and_quantifiers),
					TEST(MultipleStubbing::stub_multiple_return_values_with_quantifier),
					TEST(MultipleStubbing::stub_multiple_throws_and_returns)
					//
							) {
	}

	struct SomeInterface {
		virtual int func(int) = 0;
		virtual void proc(int) = 0;
	};

	void stub_multiple_return_values_with_quantifier() {
		Mock<SomeInterface> mock;
		When(Method(mock,func)).Return(Times<2>::of(1), Times<2>::of(2));
		When(Method(mock,proc)).Return(Times<2>::Void());

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(2, i.func(1));
		ASSERT_EQUAL(2, i.func(1));
		ASSERT_THROW(i.func(1), fakeit::UnexpectedMethodCallException);

		i.proc(1);
		i.proc(1);
		ASSERT_THROW(i.proc(1), fakeit::UnexpectedMethodCallException);
	}

	void stub_multiple_return_values_with_mixed_values_and_quantifiers() {
		Mock<SomeInterface> mock;
		When(Method(mock,func)).Return(0, Times<2>::of(1), 2);
		SomeInterface &i = mock.get();
		ASSERT_EQUAL(0, i.func(1));
		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(2, i.func(1));
		ASSERT_THROW(i.func(1), fakeit::UnexpectedMethodCallException);
	}

	void stub_multiple_throws_with_quantifier() {
		Mock<SomeInterface> mock;
		When(Method(mock,func)).Throw(Times<2>::of(std::string("1")),Times<1>::of(1));
		When(Method(mock,proc)).Throw(Times<2>::of(std::string("1")),Times<1>::of(1));
		SomeInterface &i = mock.get();
//
		ASSERT_THROW(i.func(1), std::string);
		ASSERT_THROW(i.func(1), std::string);
		ASSERT_THROW(i.func(1), int);
		ASSERT_THROW(i.func(1), fakeit::UnexpectedMethodCallException);

		ASSERT_THROW(i.proc(1), std::string);
		ASSERT_THROW(i.proc(1), std::string);
		ASSERT_THROW(i.proc(1), int);
		ASSERT_THROW(i.proc(1), fakeit::UnexpectedMethodCallException);
	}

	void stub_multiple_throws_and_returns() {
		Mock<SomeInterface> mock;
		When(Method(mock,func)).Throw(std::string("1"),Times<1>::of(1)).Return(1,Times<1>::of(2));
		When(Method(mock,proc)).Throw(std::string("1"),Times<1>::of(1)).Return(Times<2>::Void());
		SomeInterface &i = mock.get();
//
		ASSERT_THROW(i.func(1), std::string);
		ASSERT_THROW(i.func(1), int);
		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(2, i.func(1));
		ASSERT_THROW(i.func(1), fakeit::UnexpectedMethodCallException);

		ASSERT_THROW(i.proc(1), std::string);
		ASSERT_THROW(i.proc(1), int);
		i.proc(1);
		i.proc(2);
		ASSERT_THROW(i.proc(1), fakeit::UnexpectedMethodCallException);
	}


	void stub_multiple_do_with_list_of_mixed_quatifiers_and_values() {
		Mock<SomeInterface> mock;
		When(Method(mock,func)).Do(Times<1>::of([](...){return 1;}), [](...){return 2;});
		When(Method(mock,proc)).Do(Times<1>::of([](...){}), [](...){});
		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(2, i.func(1));
		ASSERT_THROW(i.func(1), fakeit::UnexpectedMethodCallException);

		i.proc(1);
		i.proc(1);
		ASSERT_THROW(i.proc(1), fakeit::UnexpectedMethodCallException);
	}

} __MultipleStubbing;

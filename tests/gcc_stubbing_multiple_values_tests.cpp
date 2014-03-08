#if defined (__GNUG__)

#include <string>
#include <queue>

#include "tpunit++.hpp"
#include "fakeit.h"
#include "fakeit/Exceptions.h"

using namespace fakeit;

struct GccMultipleStubbing: tpunit::TestFixture {
	GccMultipleStubbing() :
			tpunit::TestFixture(
					//
					TEST(GccMultipleStubbing::stub_multiple_do_with_list_of_mixed_quatifiers_and_values),
					TEST(GccMultipleStubbing::stub_multiple_throws_with_quantifier),
					TEST(GccMultipleStubbing::stub_multiple_return_values_with_mixed_values_and_quantifiers),
					TEST(GccMultipleStubbing::stub_multiple_return_values_with_quantifier),
					TEST(GccMultipleStubbing::stub_multiple_throws_and_returns)
					//
							) {
	}

	struct SomeInterface {
		virtual int func(int) = 0;
		virtual void proc(int) = 0;
	};

	void stub_multiple_return_values_with_quantifier() {
		Mock<SomeInterface> mock;
		When(mock[&SomeInterface::func]).Return(2_Times(1), 2_Times(2));
		When(mock[&SomeInterface::proc]).Return(2_Times);

		SomeInterface &i = mock.get();
		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(2, i.func(1));
		ASSERT_EQUAL(2, i.func(1));
		ASSERT_THROW(i.func(1), fakeit::UnmockedMethodCallException);

		i.proc(1);
		i.proc(1);
		ASSERT_THROW(i.proc(1), fakeit::UnmockedMethodCallException);
	}

	void stub_multiple_return_values_with_mixed_values_and_quantifiers() {
		Mock<SomeInterface> mock;
		When(mock[&SomeInterface::func]).Return(0, 2_Times(1), 2);
		SomeInterface &i = mock.get();
		ASSERT_EQUAL(0, i.func(1));
		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(2, i.func(1));
		ASSERT_THROW(i.func(1), fakeit::UnmockedMethodCallException);
	}

	void stub_multiple_throws_with_quantifier() {
		Mock<SomeInterface> mock;
		When(mock[&SomeInterface::func]).Throw(2_Times(std::string("1")), 1_Times(1));
		When(mock[&SomeInterface::proc]).Throw(2_Times(std::string("1")), 1_Times(1));
		SomeInterface &i = mock.get();
//
		ASSERT_THROW(i.func(1), std::string);
		ASSERT_THROW(i.func(1), std::string);
		ASSERT_THROW(i.func(1), int);
		ASSERT_THROW(i.func(1), fakeit::UnmockedMethodCallException);

		ASSERT_THROW(i.proc(1), std::string);
		ASSERT_THROW(i.proc(1), std::string);
		ASSERT_THROW(i.proc(1), int);
		ASSERT_THROW(i.proc(1), fakeit::UnmockedMethodCallException);
	}

	void stub_multiple_throws_and_returns() {
		Mock<SomeInterface> mock;
		When(mock[&SomeInterface::func]).Throw(std::string("1"), 1_Times(1)).Return(1, 1_Times(2));
		When(mock[&SomeInterface::proc]).Throw(std::string("1"), 1_Times(1)).Return(2_Times);
		SomeInterface &i = mock.get();
//
		ASSERT_THROW(i.func(1), std::string);
		ASSERT_THROW(i.func(1), int);
		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(2, i.func(1));
		ASSERT_THROW(i.func(1), fakeit::UnmockedMethodCallException);

		ASSERT_THROW(i.proc(1), std::string);
		ASSERT_THROW(i.proc(1), int);
		i.proc(1);
		i.proc(2);
		ASSERT_THROW(i.proc(1), fakeit::UnmockedMethodCallException);
	}

	void stub_multiple_do_with_list_of_mixed_quatifiers_and_values() {
		Mock<SomeInterface> mock;
		When(mock[&SomeInterface::func]).Do(1_Times([](...) {return 1;}), [](...) {return 2;});
		When(mock[&SomeInterface::proc]).Do(1_Times([](...) {}), [](...) {});
		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(2, i.func(1));
		ASSERT_THROW(i.func(1), fakeit::UnmockedMethodCallException);

		i.proc(1);
		i.proc(1);
		ASSERT_THROW(i.proc(1), fakeit::UnmockedMethodCallException);
	}

} __GccMultipleStubbing;

#endif

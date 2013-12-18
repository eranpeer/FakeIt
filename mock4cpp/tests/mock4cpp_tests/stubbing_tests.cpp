#include "tpunit++.hpp"
#include "mock4cpp.h"

struct test_any: tpunit::TestFixture {
	test_any() :
			tpunit::TestFixture( //
					TEST(test_any::ExceptionWhenCallingANonStubbedMethod),//
					TEST(test_any::test_b)  //
					)  //
	{
	}

	struct SomeInterface
		{
			virtual int func(int) = 0;
			virtual void proc(int) = 0;
		};

	void ExceptionWhenCallingANonStubbedMethod()
	{
		Mock<SomeInterface> mock;
		//SomeInterface &i = mock.get();

		//Assert::ExpectException<UnmockedMethodException>([&i]{ i.func(1); });
		//Assert::ExpectException<UnmockedMethodException>([&i]{ i.proc(2); });
	}

	void test_a() {
	}

	void test_b() {
	}

} __test_any;

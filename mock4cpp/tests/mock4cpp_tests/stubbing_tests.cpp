#include "tpunit++.hpp"
#include "mock4cpp.h"
#include <string>

struct BasicStubbing: tpunit::TestFixture {
	BasicStubbing() :
			tpunit::TestFixture(
					//
					TEST(BasicStubbing::calling_an_unstubbed_method_should_raise_UnmockedMethodException), //
					TEST(BasicStubbing::stubbing_method_to_default_behaviore), //
					TEST(BasicStubbing::stubbing_multiple_methods_to_default_behaviore),
					TEST(BasicStubbing::stubbing_a_function_and_returning_a_specified_value),
					TEST(BasicStubbing::stubbing_a_method_and_throwing_a_specified_exception) //
							)  //
	{
	}

	struct SomeInterface {
		virtual int func(int) = 0;
		virtual void proc(int) = 0;
		int a = 0;
	};

	void calling_an_unstubbed_method_should_raise_UnmockedMethodException() {
		Mock<SomeInterface> mock;
		SomeInterface &i = mock.get();
		ASSERT_THROW(i.func(1), UnmockedMethodException);
		ASSERT_THROW(i.proc(1), UnmockedMethodException);
	}

	void stubbing_method_to_default_behaviore() {
		Mock<SomeInterface> mock;

		Stub(mock[&SomeInterface::func]);
		Stub(mock[&SomeInterface::proc]);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(0, i.func(1));
		ASSERT_NO_THROW(i.proc(1));
	}

	void stubbing_multiple_methods_to_default_behaviore() {
		Mock<SomeInterface> mock;

		Stub(mock[&SomeInterface::func], mock[&SomeInterface::proc]);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(0, i.func(1));
		ASSERT_NO_THROW(i.proc(1));
	}

	void stubbing_a_method_to_default_behaviore() {
		Mock<SomeInterface> mock;

		Stub(mock[&SomeInterface::func], mock[&SomeInterface::proc]);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(0, i.func(1));
		ASSERT_NO_THROW(i.proc(1));
	}

	void stubbing_a_function_and_returning_a_specified_value() {
		Mock<SomeInterface> mock;

		When(mock[&SomeInterface::func]).Return(1);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(1));
	}

	void stubbing_a_method_and_throwing_a_specified_exception() {
		Mock<SomeInterface> mock;

		When(mock[&SomeInterface::func]).Throw(std::string("exception"));
		When(mock[&SomeInterface::proc]).Throw(std::string("exception"));

		SomeInterface &i = mock.get();

		try {
			i.func(1);
		} catch (std::string e) {
			ASSERT_EQUAL(std::string("exception"), e);
		}

		try {
			i.proc(1);
		} catch (std::string e) {
			ASSERT_EQUAL(std::string("exception"), e);
		}
	}

	void test_a() {
		//mock[&SomeInterface::func] = [](...){return 0;};
//			mock.When(&SomeInterface::func).Return(0);
//			mock.When(&SomeInterface::func).Do([](...){return 0;});
//			mock.When(&SomeInterface::proc).Return();
//			mock.When(&SomeInterface::proc).Do([](...){return ;});
//			mock.When(&SomeInterface::proc).Throw(1);
//			mock[&SomeInterface::proc];
//			mock[&SomeInterface::proc] = [](...){return ;};
//			mock[&SomeInterface::func] = [](...){return 1;};

	}

	void test_b() {
	}

} __test_any;

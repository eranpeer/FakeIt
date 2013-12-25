#include "tpunit++.hpp"
#include "mock4cpp.h"
#include <string>
#include "mock4cpp/UnmockedMethodCallException.h"

struct BasicStubbing: tpunit::TestFixture {
	BasicStubbing() :
			tpunit::TestFixture(
					//
					TEST(BasicStubbing::calling_an_unstubbed_method_should_raise_UnmockedMethodCallException), //
					TEST(BasicStubbing::stub_method_to_default_behaviore), //
					TEST(BasicStubbing::stub_multiple_methods_to_default_behaviore),
					TEST(BasicStubbing::stub_a_function_to_return_a_specified_value),
					TEST(BasicStubbing::stub_a_method_to_throw_a_specified_exception), //
					TEST(BasicStubbing::stub_a_method_with_lambda_delegate), //
					TEST(BasicStubbing::stub_a_method_with_static_method_delegate), //
					TEST(BasicStubbing::stub_by_assignment_with_lambda_delegate), //
					TEST(BasicStubbing::stub_by_assignment_with_static_method_delegate), //
					TEST(BasicStubbing::consditional_stubbing)
							)  //
	{
	}

	struct SomeInterface {
		virtual int func(int) = 0;
		virtual void proc(int) = 0;
		int a = 0;
	};

	void calling_an_unstubbed_method_should_raise_UnmockedMethodCallException() {
		Mock<SomeInterface> mock;
		SomeInterface &i = mock.get();
		ASSERT_THROW(i.func(1), UnmockedMethodCallException);
		ASSERT_THROW(i.proc(1), UnmockedMethodCallException);
	}

	void stub_method_to_default_behaviore() {
		Mock<SomeInterface> mock;

		Stub(mock[&SomeInterface::func]);
		Stub(mock[&SomeInterface::proc]);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(0, i.func(1));
		ASSERT_NO_THROW(i.proc(1));
	}

	void stub_multiple_methods_to_default_behaviore() {
		Mock<SomeInterface> mock;

		Stub(mock[&SomeInterface::func], mock[&SomeInterface::proc]);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(0, i.func(1));
		ASSERT_NO_THROW(i.proc(1));
	}

	void stub_a_function_to_return_a_specified_value() {
		Mock<SomeInterface> mock;

		When(mock[&SomeInterface::func]).Return(1);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(1));
	}

	void stub_a_method_to_throw_a_specified_exception() {
		Mock<SomeInterface> mock;

		When(mock[&SomeInterface::func]).Throw(std::string("func exception"));
		When(mock[&SomeInterface::proc]).Throw(std::string("proc exception"));

		SomeInterface &i = mock.get();

		try {
			i.func(1);
		} catch (std::string e) {
			ASSERT_EQUAL(std::string("func exception"), e);
		}

		try {
			i.proc(1);
		} catch (std::string e) {
			ASSERT_EQUAL(std::string("proc exception"), e);
		}
	}

	void stub_a_method_with_lambda_delegate() {
		Mock<SomeInterface> mock;

		int a = 0;

		When(mock[&SomeInterface::func]).Do([](int val) {return val;});
		When(mock[&SomeInterface::proc]).Do([&a](int val) {a = val;});

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(3, i.func(3));

		i.proc(3);
		ASSERT_EQUAL(3, a);
	}

	static int func_delegate(int val) {
		return val;
	}

	static void proc_delegate(int val) {
		throw val;
	}

	void stub_a_method_with_static_method_delegate() {
		Mock<SomeInterface> mock;

		When(mock[&SomeInterface::func]).Do(func_delegate);
		When(mock[&SomeInterface::proc]).Do(proc_delegate);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(3, i.func(3));

		try {
			i.proc(1);
		} catch (int e) {
			ASSERT_EQUAL(1, e);
		}
	}

	void stub_by_assignment_with_lambda_delegate() {
		Mock<SomeInterface> mock;

		int a = 0;

		mock[&SomeInterface::func] = [](int val) {return val;};
		mock[&SomeInterface::proc] = [&a](int val) {a = val;};

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(3, i.func(3));

		i.proc(3);
		ASSERT_EQUAL(3, a);
	}

	void stub_by_assignment_with_static_method_delegate() {
		Mock<SomeInterface> mock;

		mock[&SomeInterface::func] = func_delegate;
		mock[&SomeInterface::proc] = proc_delegate;

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(3, i.func(3));

		try {
			i.proc(1);
		} catch (int e) {
			ASSERT_EQUAL(1, e);
		}
	}

	void consditional_stubbing() {
		Mock<SomeInterface> mock;

		Stub(mock[&SomeInterface::func].Using(1));
		Stub(mock[&SomeInterface::proc].Using(1));

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(0, i.func(1));
		ASSERT_THROW(i.func(2),mock4cpp::UnmockedMethodCallException);

		i.proc(1);
		ASSERT_THROW(i.proc(2),mock4cpp::UnmockedMethodCallException);

	}

} __BasicStubbing;

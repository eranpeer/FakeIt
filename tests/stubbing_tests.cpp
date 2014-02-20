#include "tpunit++.hpp"
#include "fakeit.h"
#include <string>
#include "fakeit/Exceptions.h"
#include <queue>

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
					TEST(BasicStubbing::stub_to_default_behavior_with_filter), //
					TEST(BasicStubbing::change_method_behavior_with_filter), //
					TEST(BasicStubbing::change_method_behavior_with_functor_filter), //
					TEST(BasicStubbing::change_method_behavior_with_matcher),  //
					TEST(BasicStubbing::change_method_behavior_with_functor_matcher),  //
					TEST(BasicStubbing::stub_multiple_return_values), //
					TEST(BasicStubbing::stub_multiple_return_values_using_quque), //
					TEST(BasicStubbing::stub_multiple_throws)
					//
							) {
	}

	struct SomeInterface {
		virtual int func(int) = 0;
		virtual void proc(int) = 0;
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
//
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

	void stub_to_default_behavior_with_filter() {
		Mock<SomeInterface> mock;

		Stub(mock[&SomeInterface::func].Using(1));
		Stub(mock[&SomeInterface::proc].Using(1));

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(0, i.func(1));
		ASSERT_THROW(i.func(2), fakeit::UnmockedMethodCallException);

		i.proc(1);
		ASSERT_THROW(i.proc(2), fakeit::UnmockedMethodCallException);
	}

	void change_method_behavior_with_filter() {
		class Exc: public std::exception {
		} e;

		Mock<SomeInterface> mock;

		When(mock[&SomeInterface::func].Using(1)).Return(1);
		When(mock[&SomeInterface::proc].Using(1)).Throw(e);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(1));
		ASSERT_THROW(i.func(2), fakeit::UnmockedMethodCallException);

		ASSERT_THROW(i.proc(1), Exc);
		ASSERT_THROW(i.proc(2), fakeit::UnmockedMethodCallException);
	}

	void change_method_behavior_with_functor_filter() {
		class Exc: public std::exception {
		} e;

		Mock<SomeInterface> mock;

		When(mock[&SomeInterface::func](1)).Return(1);
		When(mock[&SomeInterface::proc](1)).Throw(e);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(1));
		ASSERT_THROW(i.func(2), fakeit::UnmockedMethodCallException);

		ASSERT_THROW(i.proc(1), Exc);
		ASSERT_THROW(i.proc(2), fakeit::UnmockedMethodCallException);
	}

	void change_method_behavior_with_matcher() {
		class Exc: public std::exception {
		} e;

		Mock<SomeInterface> mock;

		When(mock[&SomeInterface::func].Matching([](int a) {return a == 1;})).Return(1);
		When(mock[&SomeInterface::proc].Matching([](int a) {return a == 1;})).Throw(e);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(1));
		ASSERT_THROW(i.func(2), fakeit::UnmockedMethodCallException);

		ASSERT_THROW(i.proc(1), Exc);
		ASSERT_THROW(i.proc(2), fakeit::UnmockedMethodCallException);
	}

	void change_method_behavior_with_functor_matcher() {
		class Exc: public std::exception {
		} e;

		Mock<SomeInterface> mock;

		When(mock[&SomeInterface::func]([](int a) {return a == 1;})).Return(1);
		When(mock[&SomeInterface::proc]([](int a) {return a == 1;})).Throw(e);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(1));
		ASSERT_THROW(i.func(2), fakeit::UnmockedMethodCallException);

		ASSERT_THROW(i.proc(1), Exc);
		ASSERT_THROW(i.proc(2), fakeit::UnmockedMethodCallException);
	}

	void stub_multiple_return_values() {
		Mock<SomeInterface> mock;

		When(mock[&SomeInterface::func]).Return(1).Return(2);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(0));
		ASSERT_EQUAL(2, i.func(0));
	}

	void stub_multiple_return_values_using_quque() {
		Mock<SomeInterface> mock;
		std::queue<int> q( { 1, 2 });

		When(mock[&SomeInterface::func]).AlwaysDo([&](...) {int rv = q.front(); q.pop(); return rv;});

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(0));
		ASSERT_EQUAL(2, i.func(0));
	}
//
	void stub_multiple_throws() {

		Mock<SomeInterface> mock;

		When(mock[&SomeInterface::func]).Throw(std::string("A")).Throw(std::string("B"));
		When(mock[&SomeInterface::proc]).Throw(std::string("A")).Throw(std::string("B"));

		SomeInterface &i = mock.get();

		try {
			i.func(0);
		} catch (std::string & e) {
			ASSERT_EQUAL(std::string("A"), e);
		}

		try {
			i.func(0);
		} catch (std::string & e) {
			ASSERT_EQUAL(std::string("B"), e);
		}

		try {
			i.proc(0);
		} catch (std::string & e) {
			ASSERT_EQUAL(std::string("A"), e);
		}

		try {
			i.proc(0);
		} catch (std::string & e) {
			ASSERT_EQUAL(std::string("B"), e);
		}
	}

} __BasicStubbing;

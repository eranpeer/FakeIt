#include <string>
#include <queue>
#include <iostream>

#include "tpunit++.hpp"
#include "fakeit.h"
#include "fakeit/Exceptions.h"

using namespace fakeit;
using namespace std;

struct DemoTests : tpunit::TestFixture {
	DemoTests() :
	tpunit::TestFixture(
	//
// 	TEST(DemoTests::calling_an_unstubbed_method_should_raise_UnmockedMethodCallException), //
// 	TEST(DemoTests::stub_method_to_default_behaviore_will_always_return_do_the_default_behaviore), //
// 	TEST(DemoTests::stub_multiple_methods_to_default_behaviore),
 	TEST(DemoTests::basic_stubbing),
	TEST(DemoTests::basic_verification)
// 	TEST(DemoTests::stub_a_function_to_return_a_specified_value_always),
// 	TEST(DemoTests::stub_a_method_to_throw_a_specified_exception_once), //
// 	TEST(DemoTests::stub_a_method_with_lambda_delegate_once), //
// 	TEST(DemoTests::stub_a_method_with_lambda_delegate_always), //
// 	TEST(DemoTests::stub_a_method_with_static_method_delegate), //
// 	TEST(DemoTests::stub_by_assignment_with_lambda_delegate), //
// 	TEST(DemoTests::stub_by_assignment_with_static_method_delegate), //
// 	TEST(DemoTests::stub_to_default_behavior_with_filter), //
// 	TEST(DemoTests::change_method_behavior_with_filter), //
// 	TEST(DemoTests::change_method_behavior_with_functor_filter), //
// 	TEST(DemoTests::change_method_behavior_with_matcher),  //
// 	TEST(DemoTests::change_method_behavior_with_functor_matcher),  //
// 	TEST(DemoTests::stub_multiple_return_values), //
// 	TEST(DemoTests::stub_multiple_return_values_using_quque), //
// 	TEST(DemoTests::stub_multiple_throws),
// 	TEST(DemoTests::stub_overloaded_methods)
	//
	) {
	}

	struct SomeInterface {
		virtual int foo(int) = 0;
		virtual int bar(string) = 0;
	};

// 	void calling_an_unstubbed_method_should_raise_UnmockedMethodCallException() {
// 		Mock<SomeInterface> mock;
// 		SomeInterface &i = mock.get();
// 		ASSERT_THROW(i.bar(1), UnmockedMethodCallException);
// 		ASSERT_THROW(i.foo(1), UnmockedMethodCallException);
// 	}
// 
// 	void stub_method_to_default_behaviore_will_always_return_do_the_default_behaviore() {
// 		Mock<SomeInterface> mock;
// 
// 		Stub(mock[&SomeInterface::bar]);
// 		Stub(mock[&SomeInterface::foo]);
// 
// 		SomeInterface &i = mock.get();
// 
// 		ASSERT_EQUAL(0, i.bar(1));
// 		ASSERT_NO_THROW(i.foo(1));
// 
// 		ASSERT_EQUAL(0, i.bar(1));
// 		ASSERT_NO_THROW(i.foo(1));
// 	}
// 
// 	void stub_multiple_methods_to_default_behaviore() {
// 		Mock<SomeInterface> mock;
// 
// 		Stub(mock[&SomeInterface::bar], mock[&SomeInterface::foo]);
// 
// 		SomeInterface &i = mock.get();
// 
// 		ASSERT_EQUAL(0, i.bar(1));
// 		ASSERT_NO_THROW(i.foo(1));
// 
// 		ASSERT_EQUAL(0, i.bar(1));
// 		ASSERT_NO_THROW(i.foo(1));
// 	}
// 
	void basic_stubbing() {
		// Instantiate a mock object.
		Mock<SomeInterface> mock;

		// Setup mock behavior.
		When(mock[&SomeInterface::foo]).AlwaysReturn(1);

		// Fetch the mock instance.
		SomeInterface &i = mock.get();
		
		// Will print "1". 
		cout << i.foo(0);
	}

	void basic_verification(){
		Mock<SomeInterface> mock;
		
		When(mock[&SomeInterface::foo]).AlwaysReturn(0);
		When(mock[&SomeInterface::bar]).AlwaysReturn(0);

		When(mock[&SomeInterface::foo]).Return(0, Times<4>::of(3), 2, 3);

		SomeInterface &i = mock.get();
		
 		// Production code
		i.foo(1);
 		i.bar("some value");

		// Verify for foo & bar where invoked
 		Verify(mock[&SomeInterface::foo]);
 		Verify(mock[&SomeInterface::bar]);

		// Verify for foo & bar where invoked with specific arguments
		Verify(mock[&SomeInterface::foo].Using(1));
		Verify(mock[&SomeInterface::bar].Using("some value"));

		// Verify for foo & bar where never invoked with other arguments
		Verify(mock[&SomeInterface::foo].Using(2)).Never();
		Verify(mock[&SomeInterface::bar].Using("some other value")).Never();
	}
// 
// 	void stub_a_function_to_return_a_specified_value_always() {
// 		Mock<SomeInterface> mock;
// 
// 		When(mock[&SomeInterface::bar]).AlwaysReturn(1);
// 
// 		SomeInterface &i = mock.get();
// 
// 		ASSERT_EQUAL(1, i.bar(1));
// 		ASSERT_EQUAL(1, i.bar(1));
// 	}
// 
// 
// 	void stub_a_method_to_throw_a_specified_exception_once() {
// 		Mock<SomeInterface> mock;
// 
// 		When(mock[&SomeInterface::bar]).Throw(std::string("func exception"));
// 		When(mock[&SomeInterface::foo]).Throw(std::string("proc exception"));
// 
// 		SomeInterface &i = mock.get();
// 
// 		try {
// 			i.bar(1);
// 		}
// 		catch (std::string e) {
// 			ASSERT_EQUAL(std::string("func exception"), e);
// 		}
// 
// 		try {
// 			i.foo(1);
// 		}
// 		catch (std::string e) {
// 			ASSERT_EQUAL(std::string("proc exception"), e);
// 		}
// 
// 		ASSERT_THROW(i.bar(1), fakeit::UnmockedMethodCallException);
// 		ASSERT_THROW(i.foo(1), fakeit::UnmockedMethodCallException);
// 	}
// 
// 	void stub_a_method_with_lambda_delegate_once() {
// 		Mock<SomeInterface> mock;
// 
// 		int a = 0;
// 
// 		When(mock[&SomeInterface::bar]).Do([](int val) {return val; });
// 		When(mock[&SomeInterface::foo]).Do([&a](int val) {a = val; });
// 
// 		SomeInterface &i = mock.get();
// 
// 		ASSERT_EQUAL(3, i.bar(3));
// 
// 		i.foo(3);
// 		ASSERT_EQUAL(3, a);
// 
// 		ASSERT_THROW(i.bar(3), fakeit::UnmockedMethodCallException);
// 		ASSERT_THROW(i.foo(3), fakeit::UnmockedMethodCallException);
// 	}
// 
// 	void stub_a_method_with_lambda_delegate_always() {
// 		Mock<SomeInterface> mock;
// 
// 		int a = 0;
// 
// 		When(mock[&SomeInterface::bar]).AlwaysDo([](int val) {return val; });
// 		When(mock[&SomeInterface::foo]).AlwaysDo([&a](int val) {a = val; });
// 
// 		SomeInterface &i = mock.get();
// 
// 		ASSERT_EQUAL(3, i.bar(3));
// 		ASSERT_EQUAL(3, i.bar(3));
// 
// 		i.foo(3);
// 		ASSERT_EQUAL(3, a);
// 
// 		a = 0;
// 		i.foo(3);
// 		ASSERT_EQUAL(3, a);
// 	}
// 
// 	static int func_delegate(int val) {
// 		return val;
// 	}
// 
// 	static void proc_delegate(int val) {
// 		throw val;
// 	}
// 
// 	void stub_a_method_with_static_method_delegate() {
// 		Mock<SomeInterface> mock;
// 
// 		When(mock[&SomeInterface::bar]).Do(func_delegate);
// 		When(mock[&SomeInterface::foo]).Do(proc_delegate);
// 
// 		SomeInterface &i = mock.get();
// 
// 		ASSERT_EQUAL(3, i.bar(3));
// 
// 		try {
// 			i.foo(1);
// 		}
// 		catch (int e) {
// 			ASSERT_EQUAL(1, e);
// 		}
// 	}
// 
// 	void stub_by_assignment_with_lambda_delegate() {
// 		Mock<SomeInterface> mock;
// 
// 		int a = 0;
// 
// 		mock[&SomeInterface::bar] = [](int val) {return val; };
// 		mock[&SomeInterface::foo] = [&a](int val) {a = val; };
// 
// 		SomeInterface &i = mock.get();
// 
// 		ASSERT_EQUAL(3, i.bar(3));
// 
// 		i.foo(3);
// 		ASSERT_EQUAL(3, a);
// 	}
// 
// 	void stub_by_assignment_with_static_method_delegate() {
// 		Mock<SomeInterface> mock;
// 
// 		mock[&SomeInterface::bar] = func_delegate;
// 		mock[&SomeInterface::foo] = proc_delegate;
// 
// 		SomeInterface &i = mock.get();
// 
// 		ASSERT_EQUAL(3, i.bar(3));
// 
// 		try {
// 			i.foo(1);
// 		}
// 		catch (int e) {
// 			ASSERT_EQUAL(1, e);
// 		}
// 	}
// 
// 	void stub_to_default_behavior_with_filter() {
// 		Mock<SomeInterface> mock;
// 
// 		Stub(mock[&SomeInterface::bar].Using(1));
// 		Stub(mock[&SomeInterface::foo].Using(1));
// 
// 		SomeInterface &i = mock.get();
// 
// 		ASSERT_EQUAL(0, i.bar(1));
// 		ASSERT_THROW(i.bar(2), fakeit::UnmockedMethodCallException);
// 
// 		i.foo(1);
// 		ASSERT_THROW(i.foo(2), fakeit::UnmockedMethodCallException);
// 	}
// 
// 	void change_method_behavior_with_filter() {
// 		class Exc : public std::exception {
// 		} e;
// 
// 		Mock<SomeInterface> mock;
// 
// 		When(mock[&SomeInterface::bar].Using(1)).Return(1);
// 		When(mock[&SomeInterface::foo].Using(1)).Throw(e);
// 
// 		SomeInterface &i = mock.get();
// 
// 		ASSERT_EQUAL(1, i.bar(1));
// 		ASSERT_THROW(i.bar(2), fakeit::UnmockedMethodCallException);
// 
// 		ASSERT_THROW(i.foo(1), Exc);
// 		ASSERT_THROW(i.foo(2), fakeit::UnmockedMethodCallException);
// 	}
// 
// 	void change_method_behavior_with_functor_filter() {
// 		class Exc : public std::exception {
// 		} e;
// 
// 		Mock<SomeInterface> mock;
// 
// 		When(mock[&SomeInterface::bar](1)).Return(1);
// 		When(mock[&SomeInterface::foo](1)).Throw(e);
// 
// 		SomeInterface &i = mock.get();
// 
// 		ASSERT_EQUAL(1, i.bar(1));
// 		ASSERT_THROW(i.bar(2), fakeit::UnmockedMethodCallException);
// 
// 		ASSERT_THROW(i.foo(1), Exc);
// 		ASSERT_THROW(i.foo(2), fakeit::UnmockedMethodCallException);
// 	}
// 
// 	void change_method_behavior_with_matcher() {
// 		class Exc : public std::exception {
// 		} e;
// 
// 		Mock<SomeInterface> mock;
// 
// 		When(mock[&SomeInterface::bar].Matching([](int a) {return a == 1; })).Return(1);
// 		When(mock[&SomeInterface::foo].Matching([](int a) {return a == 1; })).Throw(e);
// 
// 		SomeInterface &i = mock.get();
// 
// 		ASSERT_EQUAL(1, i.bar(1));
// 		ASSERT_THROW(i.bar(2), fakeit::UnmockedMethodCallException);
// 
// 		ASSERT_THROW(i.foo(1), Exc);
// 		ASSERT_THROW(i.foo(2), fakeit::UnmockedMethodCallException);
// 	}
// 
// 	void change_method_behavior_with_functor_matcher() {
// 		class Exc : public std::exception {
// 		} e;
// 
// 		Mock<SomeInterface> mock;
// 
// 		When(mock[&SomeInterface::bar]([](int a) {return a == 1; })).Return(1);
// 		When(mock[&SomeInterface::foo]([](int a) {return a == 1; })).Throw(e);
// 
// 		SomeInterface &i = mock.get();
// 
// 		ASSERT_EQUAL(1, i.bar(1));
// 		ASSERT_THROW(i.bar(2), fakeit::UnmockedMethodCallException);
// 
// 		ASSERT_THROW(i.foo(1), Exc);
// 		ASSERT_THROW(i.foo(2), fakeit::UnmockedMethodCallException);
// 	}
// 
// 	void stub_multiple_return_values() {
// 		Mock<SomeInterface> mock;
// 
// 		When(mock[&SomeInterface::bar]).Return(1).Return(2);
// 
// 		SomeInterface &i = mock.get();
// 
// 		ASSERT_EQUAL(1, i.bar(0));
// 		ASSERT_EQUAL(2, i.bar(0));
// 
// 		ASSERT_THROW(i.bar(0), fakeit::UnmockedMethodCallException);
// 		ASSERT_THROW(i.bar(0), fakeit::UnmockedMethodCallException);
// 	}
// 
// 	void stub_multiple_return_values_using_quque() {
// 		Mock<SomeInterface> mock;
// 		std::queue<int> q({ 1, 2 });
// 
// 		When(mock[&SomeInterface::bar]).AlwaysDo([&](...) {int rv = q.front(); q.pop(); return rv; });
// 
// 		SomeInterface &i = mock.get();
// 
// 		ASSERT_EQUAL(1, i.bar(0));
// 		ASSERT_EQUAL(2, i.bar(0));
// 	}
// 	//
// 	void stub_multiple_throws() {
// 
// 		Mock<SomeInterface> mock;
// 
// 		When(mock[&SomeInterface::bar]).Throw(std::string("A")).Throw(std::string("B"));
// 		When(mock[&SomeInterface::foo]).Throw(std::string("A")).Throw(std::string("B"));
// 
// 		SomeInterface &i = mock.get();
// 
// 		try {
// 			i.bar(0);
// 		}
// 		catch (std::string & e) {
// 			ASSERT_EQUAL(std::string("A"), e);
// 		}
// 
// 		try {
// 			i.bar(0);
// 		}
// 		catch (std::string & e) {
// 			ASSERT_EQUAL(std::string("B"), e);
// 		}
// 
// 		try {
// 			i.foo(0);
// 		}
// 		catch (std::string & e) {
// 			ASSERT_EQUAL(std::string("A"), e);
// 		}
// 
// 		try {
// 			i.foo(0);
// 		}
// 		catch (std::string & e) {
// 			ASSERT_EQUAL(std::string("B"), e);
// 		}
// 	}
// 
// 	struct OverloadedInterface {
// 		virtual int func() = 0;
// 		virtual int func(int) = 0;
// 		virtual int func(bool) = 0;
// 	};
// 
// 	void stub_overloaded_methods() {
// 		Mock<OverloadedInterface> mock;
// 
// 		int (OverloadedInterface::*void_method_ptr)() = &OverloadedInterface::func;
// 		int (OverloadedInterface::*int_method_ptr)(int) = &OverloadedInterface::func;
// 		int (OverloadedInterface::*bool_method_ptr)(bool) = &OverloadedInterface::func;
// 
// 		Stub(mock[void_method_ptr]);
// 		Stub(mock[int_method_ptr]);
// 		Stub(mock[bool_method_ptr]);
// 
// 		OverloadedInterface &i = mock.get();
// 
// 		ASSERT_EQUAL(0, i.func());
// 		ASSERT_EQUAL(0, i.func(1));
// 		ASSERT_EQUAL(0, i.func(true));
// 	}

} __DemoTests;

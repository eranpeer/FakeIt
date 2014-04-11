#include <string>
#include <queue>

#include "tpunit++.hpp"
#include "fakeit.hpp"
#include "fakeit/FakeitExceptions.hpp"

using namespace fakeit;

struct BasicStubbing: tpunit::TestFixture {
	BasicStubbing() :
	tpunit::TestFixture(
			//
			TEST(BasicStubbing::calling_an_unstubbed_method_should_raise_UnmockedMethodCallException),//
			TEST(BasicStubbing::stub_method_to_default_behaviore_will_always_return_do_the_default_behaviore),//
			TEST(BasicStubbing::stub_multiple_methods_to_default_behaviore),
			TEST(BasicStubbing::stub_a_function_to_return_a_specified_value_once),
			TEST(BasicStubbing::stub_a_function_to_return_a_specified_value_always),
			TEST(BasicStubbing::stub_a_method_to_throw_a_specified_exception_once),//
			TEST(BasicStubbing::stub_a_method_with_lambda_delegate_once),//
			TEST(BasicStubbing::stub_a_method_with_lambda_delegate_always),//
			TEST(BasicStubbing::stub_a_method_with_static_method_delegate),//
			TEST(BasicStubbing::stub_by_assignment_with_lambda_delegate),//
			TEST(BasicStubbing::stub_by_assignment_with_static_method_delegate),//
			TEST(BasicStubbing::stub_only_specified_calls_by_assignment),
			TEST(BasicStubbing::stub_return_value_by_assignment),
			TEST(BasicStubbing::stub_to_default_behavior_with_filter),//
			TEST(BasicStubbing::change_method_behavior_with_filter),//
			TEST(BasicStubbing::change_method_behavior_with_functor_filter),//
			TEST(BasicStubbing::change_method_behavior_with_matcher),//
			TEST(BasicStubbing::change_method_behavior_with_functor_matcher),//
			TEST(BasicStubbing::stub_multiple_return_values),//
			TEST(BasicStubbing::stub_multiple_return_values_using_quque),//
			TEST(BasicStubbing::stub_multiple_throws),
			TEST(BasicStubbing::stub_overloaded_methods),
			TEST(BasicStubbing::stub_multiple_return_values_with_list),
			TEST(BasicStubbing::stub_multiple_throws_with_list),
			TEST(BasicStubbing::stub_multiple_do_with_list),
			TEST(BasicStubbing::verify_all_functions_are_stubbed_correctly),
			TEST(BasicStubbing::exception_while_stubbing_should_cancel_stubbing),
			TEST(BasicStubbing::reset_mock_to_initial_state)
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
		try {
			i.func(1);
			FAIL();
		} catch(UnexpectedMethodCallException& e)
		{
//			std::string expected("Unexpected method call. Could not find recorded implementation to support method call");
//			std::string actual(e.what());
//			ASSERT_EQUAL(expected,actual);
		}
	}

	void stub_method_to_default_behaviore_will_always_return_do_the_default_behaviore() {
		Mock<SomeInterface> mock;

		Fake(mock[&SomeInterface::func]);
		Fake(mock[&SomeInterface::proc]);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(0, i.func(1));
		ASSERT_NO_THROW(i.proc(1));

		ASSERT_EQUAL(0, i.func(1));
		ASSERT_NO_THROW(i.proc(1));
	}

	void stub_multiple_methods_to_default_behaviore() {
		Mock<SomeInterface> mock;

		Fake(mock[&SomeInterface::func], mock[&SomeInterface::proc]);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(0, i.func(1));
		ASSERT_NO_THROW(i.proc(1));

		ASSERT_EQUAL(0, i.func(1));
		ASSERT_NO_THROW(i.proc(1));
	}

	void stub_a_function_to_return_a_specified_value_once() {
		Mock<SomeInterface> mock;

		When(mock[&SomeInterface::func]).Return(1);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(1));
		ASSERT_THROW(i.func(1), fakeit::UnexpectedMethodCallException);

	}

	void stub_a_function_to_return_a_specified_value_always() {
		Mock<SomeInterface> mock;

		When(mock[&SomeInterface::func]).AlwaysReturn(1);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(1, i.func(1));
	}

	void stub_a_method_to_throw_a_specified_exception_once() {
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

		ASSERT_THROW(i.func(1), fakeit::UnexpectedMethodCallException);
		ASSERT_THROW(i.proc(1), fakeit::UnexpectedMethodCallException);
	}

	void stub_a_method_with_lambda_delegate_once() {
		Mock<SomeInterface> mock;

		int a = 0;

		When(mock[&SomeInterface::func]).Do([](int val) {return val;});
		When(mock[&SomeInterface::proc]).Do([&a](int val) {a = val;});

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(3, i.func(3));

		i.proc(3);
		ASSERT_EQUAL(3, a);

		ASSERT_THROW(i.func(3), fakeit::UnexpectedMethodCallException);
		ASSERT_THROW(i.proc(3), fakeit::UnexpectedMethodCallException);
	}

	void stub_a_method_with_lambda_delegate_always() {
		Mock<SomeInterface> mock;

		int a = 0;

		When(mock[&SomeInterface::func]).AlwaysDo([](int val) {return val;});
		When(mock[&SomeInterface::proc]).AlwaysDo([&a](int val) {a = val;});

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(3, i.func(3));
		ASSERT_EQUAL(3, i.func(3));

		i.proc(3);
		ASSERT_EQUAL(3, a);

		a = 0;
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
		ASSERT_THROW(i.func(3), fakeit::UnexpectedMethodCallException);

		try {
			i.proc(1);
		} catch (int e) {
			ASSERT_EQUAL(1, e);
		}
		ASSERT_THROW(i.proc(1), fakeit::UnexpectedMethodCallException);
	}

	void stub_by_assignment_with_lambda_delegate() {
		Mock<SomeInterface> mock;

		int a = 0;

		mock[&SomeInterface::func] = [](int val) {return val;};
		mock[&SomeInterface::proc] = [&a](int val) {a = val;};

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(3, i.func(3));
		ASSERT_EQUAL(4, i.func(4));

		i.proc(3);
		i.proc(3);
		ASSERT_EQUAL(3, a);
	}

	void stub_only_specified_calls_by_assignment()
	{
		Mock<SomeInterface> mock;

		Fake(mock[&SomeInterface::func]);
		Fake(mock[&SomeInterface::proc]);

		mock[&SomeInterface::func].Using(1) = [](...){return 1; };
		mock[&SomeInterface::proc].Using(1) = [](...){throw std::string("error"); };

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(0, i.func(2)); // default behavior
		ASSERT_EQUAL(0, i.func(2)); // default behavior

		ASSERT_THROW(i.proc(1), std::string);
		ASSERT_THROW(i.proc(1), std::string);
		
		i.proc(2); // default behavior
		i.proc(2); // default behavior
	}

	void stub_by_assignment_with_static_method_delegate() {
		Mock<SomeInterface> mock;

		mock[&SomeInterface::func] = func_delegate;
		mock[&SomeInterface::proc] = proc_delegate;

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(3, i.func(3));
		ASSERT_EQUAL(4, i.func(4));

		try {
			i.proc(1);
		} catch (int e) {
			ASSERT_EQUAL(1, e);
		}

		try {
			i.proc(1);
		}
		catch (int e) {
			ASSERT_EQUAL(1, e);
		}
	}

	void stub_return_value_by_assignment() {
		Mock<SomeInterface> mock;

		mock[&SomeInterface::func] = 1;
		mock[&SomeInterface::func].Using(2) = 2;

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(3));
		ASSERT_EQUAL(1, i.func(4));
		ASSERT_EQUAL(2, i.func(2));
	}

	void stub_to_default_behavior_with_filter() {
		Mock<SomeInterface> mock;

		Fake(mock[&SomeInterface::func].Using(1));
		Fake(mock[&SomeInterface::proc].Using(1));

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(0, i.func(1));
		ASSERT_THROW(i.func(2), fakeit::UnexpectedMethodCallException);

		i.proc(1);
		ASSERT_THROW(i.proc(2), fakeit::UnexpectedMethodCallException);
	}

	void change_method_behavior_with_filter() {
		class Exc: public std::exception {
		} e;

		Mock<SomeInterface> mock;

		When(mock[&SomeInterface::func].Using(1)).Return(1);
		When(mock[&SomeInterface::proc].Using(1)).Throw(e);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(1));
		ASSERT_THROW(i.func(2), fakeit::UnexpectedMethodCallException);

		ASSERT_THROW(i.proc(1), Exc);
		ASSERT_THROW(i.proc(2), fakeit::UnexpectedMethodCallException);
	}

	void change_method_behavior_with_functor_filter() {
		class Exc: public std::exception {
		} e;

		Mock<SomeInterface> mock;

		When(mock[&SomeInterface::func](1)).Return(1);
		When(mock[&SomeInterface::proc](1)).Throw(e);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(1));
		ASSERT_THROW(i.func(2), fakeit::UnexpectedMethodCallException);

		ASSERT_THROW(i.proc(1), Exc);
		ASSERT_THROW(i.proc(2), fakeit::UnexpectedMethodCallException);
	}

	void change_method_behavior_with_matcher() {
		class Exc: public std::exception {
		} e;

		Mock<SomeInterface> mock;

		When(mock[&SomeInterface::func].Matching([](int a) {return a == 1;})).Return(1);
		When(mock[&SomeInterface::proc].Matching([](int a) {return a == 1;})).Throw(e);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(1));
		ASSERT_THROW(i.func(2), fakeit::UnexpectedMethodCallException);

		ASSERT_THROW(i.proc(1), Exc);
		ASSERT_THROW(i.proc(2), fakeit::UnexpectedMethodCallException);
	}

	void change_method_behavior_with_functor_matcher() {
		class Exc: public std::exception {
		} e;

		Mock<SomeInterface> mock;

		When(mock[&SomeInterface::func]([](int a) {return a == 1;})).Return(1);
		When(mock[&SomeInterface::proc]([](int a) {return a == 1;})).Throw(e);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(1));
		ASSERT_THROW(i.func(2), fakeit::UnexpectedMethodCallException);

		ASSERT_THROW(i.proc(1), Exc);
		ASSERT_THROW(i.proc(2), fakeit::UnexpectedMethodCallException);
	}

	void stub_multiple_return_values() {
		Mock<SomeInterface> mock;

		When(mock[&SomeInterface::func]).Return(1).Return(2);

		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(0));
		ASSERT_EQUAL(2, i.func(0));

		ASSERT_THROW(i.func(0), fakeit::UnexpectedMethodCallException);
		ASSERT_THROW(i.func(0), fakeit::UnexpectedMethodCallException);
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

	struct OverloadedInterface {
		virtual int func() = 0;
		virtual int func(int) = 0;
		virtual int func(bool) = 0;
	};

	void stub_overloaded_methods() {
		Mock<OverloadedInterface> mock;

		int (OverloadedInterface::*void_method_ptr)() = &OverloadedInterface::func;
		int (OverloadedInterface::*int_method_ptr)(int) = &OverloadedInterface::func;
		int (OverloadedInterface::*bool_method_ptr)(bool) = &OverloadedInterface::func;

		Fake(mock[void_method_ptr]);
		Fake(mock[int_method_ptr]);
		Fake(mock[bool_method_ptr]);

		OverloadedInterface &i = mock.get();

		ASSERT_EQUAL(0, i.func());
		ASSERT_EQUAL(0, i.func(1));
		ASSERT_EQUAL(0, i.func(true));
	}

	void stub_multiple_return_values_with_list() {
		Mock<SomeInterface> mock;
		When(mock[&SomeInterface::func]).Return(1, 2);
		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(2, i.func(1));
		ASSERT_THROW(i.func(1), fakeit::UnexpectedMethodCallException);
	}

	void stub_multiple_throws_with_list() {
		Mock<SomeInterface> mock;
		When(mock[&SomeInterface::func]).Throw(std::string("1"), std::string("2"));
		When(mock[&SomeInterface::proc]).Throw(std::string("1"), std::string("2"));
		SomeInterface &i = mock.get();

		ASSERT_THROW(i.func(1), std::string);
		ASSERT_THROW(i.func(1), std::string);
		ASSERT_THROW(i.func(1), fakeit::UnexpectedMethodCallException);

		ASSERT_THROW(i.proc(1), std::string);
		ASSERT_THROW(i.proc(1), std::string);
		ASSERT_THROW(i.proc(1), fakeit::UnexpectedMethodCallException);
	}

	void stub_multiple_do_with_list() {
		Mock<SomeInterface> mock;
		When(mock[&SomeInterface::func]).Do([](...) {return 1;}, [](...) {return 2;});
		When(mock[&SomeInterface::proc]).Do([](...) {}, [](...) {});
		SomeInterface &i = mock.get();

		ASSERT_EQUAL(1, i.func(1));
		ASSERT_EQUAL(2, i.func(1));
		ASSERT_THROW(i.func(1), fakeit::UnexpectedMethodCallException);

		i.proc(1);
		i.proc(1);
		ASSERT_THROW(i.proc(1), fakeit::UnexpectedMethodCallException);
	}

	void exception_while_stubbing_should_cancel_stubbing() {
		Mock<SomeInterface> mock;
		{
			try {
				const auto& a = When(mock[&SomeInterface::func]);
				throw std::runtime_error("some exception");
			} catch (std::exception &e) {
			}
		}
		SomeInterface &i = mock.get();
		ASSERT_THROW(i.func(1), fakeit::UnexpectedMethodCallException);
	}


	void reset_mock_to_initial_state() {
		struct SomeInterface{
			virtual int func(int) = 0;
			std::string state;
		};

		Mock<SomeInterface> mock;
		When(mock[&SomeInterface::func]).AlwaysReturn(0);
		When(mock[&SomeInterface::func].Using(1)).AlwaysReturn(1);
		mock.Stub(&SomeInterface::state, "state");

		SomeInterface& i = mock.get();
		i.func(0);
		i.func(1);

		Verify(mock[&SomeInterface::func]);
		Verify(mock[&SomeInterface::func].Using(1));

		mock.Reset();

		ASSERT_THROW(i.func(0), fakeit::UnexpectedMethodCallException);
		ASSERT_THROW(i.func(1), fakeit::UnexpectedMethodCallException);

		ASSERT_THROW(Verify(mock[&SomeInterface::func]), fakeit::VerificationException);
		ASSERT_THROW(Verify(mock[&SomeInterface::func].Using(1)), fakeit::VerificationException);


		// stub agin and check it works again.
		When(mock[&SomeInterface::func]).AlwaysReturn(0);
		When(mock[&SomeInterface::func].Using(1)).AlwaysReturn(1);

		i.func(0);
		i.func(1);

		Verify(mock[&SomeInterface::func]);
		Verify(mock[&SomeInterface::func].Using(1));
	}

	struct BigInterface {
		virtual int func1() = 0;
		virtual int func2() = 0;
		virtual int func3() = 0;
		virtual int func4() = 0;
		virtual int func5() = 0;
		virtual int func6() = 0;
		virtual int func7() = 0;
		virtual int func8() = 0;
		virtual int func9() = 0;
		virtual int func10() = 0;
		virtual int func11() = 0;
		virtual int func12() = 0;
		virtual int func13() = 0;
		virtual int func14() = 0;
		virtual int func15() = 0;
		virtual int func16() = 0;
		virtual int func17() = 0;
		virtual int func18() = 0;
		virtual int func19() = 0;
		virtual int func20() = 0;
		virtual int func21() = 0;
		virtual int func22() = 0;
		virtual int func23() = 0;
		virtual int func24() = 0;
		virtual int func25() = 0;
		virtual int func26() = 0;
		virtual int func27() = 0;
		virtual int func28() = 0;
		virtual int func29() = 0;
		virtual int func30() = 0;
		virtual int func31() = 0;
		virtual int func32() = 0;
		virtual int func33() = 0;
		virtual int func34() = 0;
		virtual int func35() = 0;
		virtual int func36() = 0;
		virtual int func37() = 0;
		virtual int func38() = 0;
		virtual int func39() = 0;
		virtual int func40() = 0;
		virtual int func41() = 0;
		virtual int func42() = 0;
		virtual int func43() = 0;
		virtual int func44() = 0;
		virtual int func45() = 0;
		virtual int func46() = 0;
		virtual int func47() = 0;
		virtual int func48() = 0;
		virtual int func49() = 0;
		virtual int func50() = 0;
	};

	void verify_all_functions_are_stubbed_correctly(){
		Mock<BigInterface> mock;
		When(mock[&BigInterface::func1]).Return(1);
		When(mock[&BigInterface::func2]).Return(2);
		When(mock[&BigInterface::func3]).Return(3);
		When(mock[&BigInterface::func4]).Return(4);
		When(mock[&BigInterface::func5]).Return(5);
		When(mock[&BigInterface::func6]).Return(6);
		When(mock[&BigInterface::func7]).Return(7);
		When(mock[&BigInterface::func8]).Return(8);
		When(mock[&BigInterface::func9]).Return(9);
		When(mock[&BigInterface::func10]).Return(10);
		When(mock[&BigInterface::func11]).Return(11);
		When(mock[&BigInterface::func12]).Return(12);
		When(mock[&BigInterface::func13]).Return(13);
		When(mock[&BigInterface::func14]).Return(14);
		When(mock[&BigInterface::func15]).Return(15);
		When(mock[&BigInterface::func16]).Return(16);
		When(mock[&BigInterface::func17]).Return(17);
		When(mock[&BigInterface::func18]).Return(18);
		When(mock[&BigInterface::func19]).Return(19);
		When(mock[&BigInterface::func20]).Return(20);
		When(mock[&BigInterface::func21]).Return(21);
		When(mock[&BigInterface::func22]).Return(22);
		When(mock[&BigInterface::func23]).Return(23);
		When(mock[&BigInterface::func24]).Return(24);
		When(mock[&BigInterface::func25]).Return(25);
		When(mock[&BigInterface::func26]).Return(26);
		When(mock[&BigInterface::func27]).Return(27);
		When(mock[&BigInterface::func28]).Return(28);
		When(mock[&BigInterface::func29]).Return(29);
		When(mock[&BigInterface::func30]).Return(30);
		When(mock[&BigInterface::func31]).Return(31);
		When(mock[&BigInterface::func32]).Return(32);
		When(mock[&BigInterface::func33]).Return(33);
		When(mock[&BigInterface::func34]).Return(34);
		When(mock[&BigInterface::func35]).Return(35);
		When(mock[&BigInterface::func36]).Return(36);
		When(mock[&BigInterface::func37]).Return(37);
		When(mock[&BigInterface::func38]).Return(38);
		When(mock[&BigInterface::func39]).Return(39);
		When(mock[&BigInterface::func40]).Return(40);
		When(mock[&BigInterface::func41]).Return(41);
		When(mock[&BigInterface::func42]).Return(42);
		When(mock[&BigInterface::func43]).Return(43);
		When(mock[&BigInterface::func44]).Return(44);
		When(mock[&BigInterface::func45]).Return(45);
		When(mock[&BigInterface::func46]).Return(46);
		When(mock[&BigInterface::func47]).Return(47);
		When(mock[&BigInterface::func48]).Return(48);
		When(mock[&BigInterface::func49]).Return(49);
		When(mock[&BigInterface::func50]).Return(50);

		BigInterface& i = mock.get();

		ASSERT_EQUAL(1,i.func1());
		ASSERT_EQUAL(2,i.func2());
		ASSERT_EQUAL(3,i.func3());
		ASSERT_EQUAL(4,i.func4());
		ASSERT_EQUAL(5,i.func5());
		ASSERT_EQUAL(6,i.func6());
		ASSERT_EQUAL(7,i.func7());
		ASSERT_EQUAL(8,i.func8());
		ASSERT_EQUAL(9,i.func9());
		ASSERT_EQUAL(10,i.func10());
		ASSERT_EQUAL(11,i.func11());
		ASSERT_EQUAL(12,i.func12());
		ASSERT_EQUAL(13,i.func13());
		ASSERT_EQUAL(14,i.func14());
		ASSERT_EQUAL(15,i.func15());
		ASSERT_EQUAL(16,i.func16());
		ASSERT_EQUAL(17,i.func17());
		ASSERT_EQUAL(18,i.func18());
		ASSERT_EQUAL(19,i.func19());
		ASSERT_EQUAL(20,i.func20());
		ASSERT_EQUAL(21,i.func21());
		ASSERT_EQUAL(22,i.func22());
		ASSERT_EQUAL(23,i.func23());
		ASSERT_EQUAL(24,i.func24());
		ASSERT_EQUAL(25,i.func25());
		ASSERT_EQUAL(26,i.func26());
		ASSERT_EQUAL(27,i.func27());
		ASSERT_EQUAL(28,i.func28());
		ASSERT_EQUAL(29,i.func29());
		ASSERT_EQUAL(30,i.func30());
		ASSERT_EQUAL(31,i.func31());
		ASSERT_EQUAL(32,i.func32());
		ASSERT_EQUAL(33,i.func33());
		ASSERT_EQUAL(34,i.func34());
		ASSERT_EQUAL(35,i.func35());
		ASSERT_EQUAL(36,i.func36());
		ASSERT_EQUAL(37,i.func37());
		ASSERT_EQUAL(38,i.func38());
		ASSERT_EQUAL(39,i.func39());
		ASSERT_EQUAL(40,i.func40());
		ASSERT_EQUAL(41,i.func41());
		ASSERT_EQUAL(42,i.func42());
		ASSERT_EQUAL(43,i.func43());
		ASSERT_EQUAL(44,i.func44());
		ASSERT_EQUAL(45,i.func45());
		ASSERT_EQUAL(46,i.func46());
		ASSERT_EQUAL(47,i.func47());
		ASSERT_EQUAL(48,i.func48());
		ASSERT_EQUAL(49,i.func49());
		ASSERT_EQUAL(50,i.func50());
	}
} __BasicStubbing;

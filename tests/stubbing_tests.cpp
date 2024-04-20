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

using namespace std::placeholders;
using namespace fakeit;

struct BasicStubbing : tpunit::TestFixture {
    BasicStubbing() :
            tpunit::TestFixture(
                    //
                    TEST(BasicStubbing::calling_an_unstubbed_method_should_raise_UnmockedMethodCallException),//
                    TEST(BasicStubbing::stub_method_to_default_behaviore_will_always_return_the_default_behaviore),//
                    TEST(BasicStubbing::stub_multiple_methods_to_default_behaviore),
                    TEST(BasicStubbing::stub_a_function_to_return_a_specified_value_once),
                    TEST(BasicStubbing::stub_a_function_to_return_a_specified_value_always),
                    TEST(BasicStubbing::stub_a_function_to_set_specified_values_once),
                    TEST(BasicStubbing::stub_a_function_to_set_specified_values_once_form2),
                    TEST(BasicStubbing::stub_a_function_to_set_specified_values_once_form2_convertible_not_assignable),
                    TEST(BasicStubbing::stub_a_function_to_set_specified_value_with_incompatible_params),
                    TEST(BasicStubbing::stub_a_function_to_set_specified_values_always),
                    TEST(BasicStubbing::stub_a_function_to_set_specified_values_always_form2),
                    TEST(BasicStubbing::stub_a_method_to_throw_a_specified_exception_once),//
                    TEST(BasicStubbing::stub_a_method_with_lambda_delegate_once),//
                    TEST(BasicStubbing::stub_a_method_with_lambda_delegate_always),//
                    TEST(BasicStubbing::stub_a_method_with_mutable_lambda_delegate_always),//
                    TEST(BasicStubbing::stub_a_method_with_static_method_delegate),//
                    TEST(BasicStubbing::stub_by_assignment_with_lambda_delegate),//
                    TEST(BasicStubbing::stub_by_assignment_with_static_method_delegate),//
                    TEST(BasicStubbing::stub_only_specified_calls_by_assignment),
                    TEST(BasicStubbing::stub_return_value_by_assignment),
                    TEST(BasicStubbing::stub_to_default_behavior_with_filter),//
                    TEST(BasicStubbing::change_method_behavior_with_filter),//
                    TEST(BasicStubbing::change_method_behavior_with_functor_filter),//
                    TEST(BasicStubbing::change_method_behavior_with_matcher),//
                    TEST(BasicStubbing::change_method_behavior_with_by_ref_arguments_matcher),//
                    TEST(BasicStubbing::change_method_behavior_with_functor_matcher),//
                    TEST(BasicStubbing::stub_multiple_return_values),//
                    TEST(BasicStubbing::stub_multiple_return_values_using_queque),//
                    TEST(BasicStubbing::stub_multiple_throws),
                    TEST(BasicStubbing::stub_multiple_return_values_with_list),
                    TEST(BasicStubbing::stub_multiple_throws_with_list),
                    TEST(BasicStubbing::stub_multiple_do_with_list),
                    TEST(BasicStubbing::exception_while_stubbing_should_cancel_stubbing),
                    TEST(BasicStubbing::reset_mock_to_initial_state),
                    TEST(BasicStubbing::use_lambda_to_change_ptr_value),
                    TEST(BasicStubbing::assingOutParamsWithLambda),
                    TEST(BasicStubbing::return_ref_to_lambda_member)
            ) {
    }

    struct SomeInterface {
        virtual int func(int) = 0;
        virtual int funcNoArgs() = 0;
        virtual int funcRefArgs(int*, int&) = 0;
        virtual int funcConvertibleNotAssignableArgs1(int&, int) = 0;
        virtual const std::string& funcRetStrRef(int) = 0;

        virtual void proc(int) = 0;
        virtual void procRefArgs(int*, int&) = 0;
        virtual void procIncompatArgs(std::string&, std::vector<std::string>&) = 0;
        virtual void procConvertibleNotAssignableArgs2(int, int&) = 0;
    };

    void calling_an_unstubbed_method_should_raise_UnmockedMethodCallException() {
        Mock<SomeInterface> mock;
        SomeInterface &i = mock.get();
        try {
            i.func(1);
            FAIL();
        } catch (UnexpectedMethodCallException &) {
        }
    }

    void stub_method_to_default_behaviore_will_always_return_the_default_behaviore() {
        Mock<SomeInterface> mock;

        Fake(Method(mock, func));
        Fake(Method(mock, proc));

        SomeInterface &i = mock.get();

        ASSERT_EQUAL(0, i.func(1));
        ASSERT_NO_THROW(i.proc(1));

        ASSERT_EQUAL(0, i.func(1));
        ASSERT_NO_THROW(i.proc(1));
    }

    void stub_multiple_methods_to_default_behaviore() {
        Mock<SomeInterface> mock;

        Fake(Method(mock, func), Method(mock, proc));

        SomeInterface &i = mock.get();

        ASSERT_EQUAL(0, i.func(1));
        ASSERT_NO_THROW(i.proc(1));

        ASSERT_EQUAL(0, i.func(1));
        ASSERT_NO_THROW(i.proc(1));
    }

    void stub_a_function_to_return_a_specified_value_once() {
        Mock<SomeInterface> mock;
        When(Method(mock, func)).Return(1);

        SomeInterface &i = mock.get();

        ASSERT_EQUAL(1, i.func(1));
        try {
            i.func(1);
            FAIL();
        } catch (fakeit::UnexpectedMethodCallException &) {
        }
    }

    void stub_a_function_to_return_a_specified_value_always() {
        Mock<SomeInterface> mock;

        When(Method(mock, func)).AlwaysReturn(1);

        SomeInterface &i = mock.get();

        ASSERT_EQUAL(1, i.func(1));
        ASSERT_EQUAL(1, i.func(1));
    }

    void stub_a_function_to_set_specified_values_once() {
        Mock<SomeInterface> mock;
        When(Method(mock, funcRefArgs)).ReturnAndSet(1, 2, 3);
        When(Method(mock, procRefArgs)).ReturnAndSet(4, 5).ReturnAndSet(6, 7).ReturnAndSet(8);

        SomeInterface &i = mock.get();

        int a = 0, b = 0;
        ASSERT_EQUAL(1, i.funcRefArgs(&a, b));
        ASSERT_EQUAL(2, a);
        ASSERT_EQUAL(3, b);
        try {
            i.funcRefArgs(&a, b);
            FAIL();
        } catch (fakeit::UnexpectedMethodCallException &) {
        }

        i.procRefArgs(&a, b);
        ASSERT_EQUAL(4, a);
        ASSERT_EQUAL(5, b);
        i.procRefArgs(&a, b);
        ASSERT_EQUAL(6, a);
        ASSERT_EQUAL(7, b);
        i.procRefArgs(&a, b);
        ASSERT_EQUAL(8, a);
        ASSERT_EQUAL(7, b);
        try {
            i.procRefArgs(&a, b);
            FAIL();
        } catch (fakeit::UnexpectedMethodCallException &) {
        }
    }

    void stub_a_function_to_set_specified_values_once_form2() {
        Mock<SomeInterface> mock;
        When(Method(mock, funcRefArgs)).ReturnAndSet(1, _2 <= 3, _1 <= 2);
        When(Method(mock, procRefArgs)).ReturnAndSet(_1 <= 4, _2 <= 5).ReturnAndSet( _2 <= 6, _1 <= 7).ReturnAndSet(_2 <= 8);

        SomeInterface &i = mock.get();

        int a = 0, b = 0;
        ASSERT_EQUAL(1, i.funcRefArgs(&a, b));
        ASSERT_EQUAL(2, a);
        ASSERT_EQUAL(3, b);
        try {
            i.funcRefArgs(&a, b);
            FAIL();
        } catch (fakeit::UnexpectedMethodCallException &) {
        }

        i.procRefArgs(&a, b);
        ASSERT_EQUAL(4, a);
        ASSERT_EQUAL(5, b);
        i.procRefArgs(&a, b);
        ASSERT_EQUAL(7, a);
        ASSERT_EQUAL(6, b);
        i.procRefArgs(&a, b);
        ASSERT_EQUAL(7, a);
        ASSERT_EQUAL(8, b);
        try {
            i.procRefArgs(&a, b);
            FAIL();
        } catch (fakeit::UnexpectedMethodCallException &) {
        }
    }

    void stub_a_function_to_set_specified_values_once_form2_convertible_not_assignable() {
        Mock<SomeInterface> mock;
        When(Method(mock, funcConvertibleNotAssignableArgs1)).ReturnAndSet(1, _1 <= 3);
        When(Method(mock, procConvertibleNotAssignableArgs2)).ReturnAndSet(_2 <= 5);

        SomeInterface &i = mock.get();

        {
            int a1 = 0, b1 = 0;
            ASSERT_EQUAL(1, i.funcConvertibleNotAssignableArgs1(a1, b1));
            ASSERT_EQUAL(3, a1);
            ASSERT_EQUAL(0, b1);
        }

        {
            int a2 = 0, b2 = 0;
            i.procConvertibleNotAssignableArgs2(a2, b2);
            ASSERT_EQUAL(0, a2);
            ASSERT_EQUAL(5, b2);
        }
    }

    void stub_a_function_to_set_specified_value_with_incompatible_params() {
        Mock<SomeInterface> mock;
        std::vector<std::string> v{"str"};
        When(Method(mock, procIncompatArgs)).ReturnAndSet(_2 <= v);

        SomeInterface &i = mock.get();

        std::string s;
        std::vector<std::string> chk_v;
        i.procIncompatArgs(s, chk_v);
        ASSERT_EQUAL(chk_v, v);

#if FAKEIT_CPLUSPLUS < 201703L || defined (_WIN32)
        When(Method(mock, procIncompatArgs)).ReturnAndSet(_1 <= v);
        try {
            i.procIncompatArgs(s, chk_v);
            FAIL();
        } catch (std::logic_error&) {
        }
#endif
    }

    void stub_a_function_to_set_specified_values_always() {
        Mock<SomeInterface> mock;

        When(Method(mock, funcRefArgs)).AlwaysReturnAndSet(1, 2, 3);
        When(Method(mock, procRefArgs)).AlwaysReturnAndSet(4, 5);

        SomeInterface &i = mock.get();

        int a = 0, b = 0;
        ASSERT_EQUAL(1, i.funcRefArgs(&a, b));
        ASSERT_EQUAL(2, a);
        ASSERT_EQUAL(3, b);

        i.procRefArgs(&a, b);
        ASSERT_EQUAL(4, a);
        ASSERT_EQUAL(5, b);

        ASSERT_EQUAL(1, i.funcRefArgs(&a, b));
        ASSERT_EQUAL(2, a);
        ASSERT_EQUAL(3, b);

        i.procRefArgs(&a, b);
        ASSERT_EQUAL(4, a);
        ASSERT_EQUAL(5, b);

    }

    void stub_a_function_to_set_specified_values_always_form2() {
        Mock<SomeInterface> mock;

        When(Method(mock, funcRefArgs)).AlwaysReturnAndSet(1, _1 <= 2, _2 <= 3);
        When(Method(mock, procRefArgs)).AlwaysReturnAndSet(_1 <= 40, _2 <= 50, _1 <= 4, _2 <= 5);

        SomeInterface &i = mock.get();

        int a = 0, b = 0;
        ASSERT_EQUAL(1, i.funcRefArgs(&a, b));
        ASSERT_EQUAL(2, a);
        ASSERT_EQUAL(3, b);

        i.procRefArgs(&a, b);
        ASSERT_EQUAL(4, a);
        ASSERT_EQUAL(5, b);

        When(Method(mock, funcRefArgs)).AlwaysReturnAndSet(1, _2 <= 3);
        ASSERT_EQUAL(1, i.funcRefArgs(&a, b));
        ASSERT_EQUAL(4, a);
        ASSERT_EQUAL(3, b);

        i.procRefArgs(&a, b);
        ASSERT_EQUAL(4, a);
        ASSERT_EQUAL(5, b);

        When(Method(mock, funcRefArgs)).AlwaysReturnAndSet(1, _1 <= 2);
        ASSERT_EQUAL(1, i.funcRefArgs(&a, b));
        ASSERT_EQUAL(2, a);
        ASSERT_EQUAL(5, b);
    }

    void stub_a_method_to_throw_a_specified_exception_once() {
        Mock<SomeInterface> mock;

        When(Method(mock, func)).Throw(std::string("func exception"));
        When(Method(mock, proc)).Throw(std::string("proc exception"));

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

        When(Method(mock, func)).Do([](int val) {
            return val;
        });
        When(Method(mock, proc)).Do([&a](int val) {
            a = val;
        });

        SomeInterface &i = mock.get();

        ASSERT_EQUAL(3, i.func(3));

        i.proc(3);
        ASSERT_EQUAL(3, a);

        ASSERT_THROW(i.func(3), fakeit::UnexpectedMethodCallException);
        ASSERT_THROW(i.proc(3), fakeit::UnexpectedMethodCallException);

		When(Method(mock, func)).Do([](int& val) {
			return val + 1;
		});
		When(Method(mock, proc)).Do([&a](int& val) {
			a = val + 1;
		});

		ASSERT_EQUAL(3 + 1, i.func(3));

		i.proc(3);
		ASSERT_EQUAL(3 + 1, a);

		ASSERT_THROW(i.func(3), fakeit::UnexpectedMethodCallException);
		ASSERT_THROW(i.proc(3), fakeit::UnexpectedMethodCallException);
	}

    void stub_a_method_with_lambda_delegate_always() {
        Mock<SomeInterface> mock;

        int a = 0;

        When(Method(mock, func)).AlwaysDo([](int val) {
            return val;
        });
        When(Method(mock, proc)).AlwaysDo([&a](int val) {
            a = val;
        });

        SomeInterface &i = mock.get();

        ASSERT_EQUAL(3, i.func(3));
        ASSERT_EQUAL(3, i.func(3));

        i.proc(3);
        ASSERT_EQUAL(3, a);

        a = 0;
        i.proc(3);
        ASSERT_EQUAL(3, a);

		When(Method(mock, func)).AlwaysDo([](int& val) {
			return val + 1;
		});
		When(Method(mock, proc)).AlwaysDo([&a](int& val) {
			a = val + 1;
		});

		ASSERT_EQUAL(3 + 1, i.func(3));
		ASSERT_EQUAL(3 + 1, i.func(3));

		i.proc(3);
		ASSERT_EQUAL(3 + 1, a);

		a = 0;
		i.proc(3);
		ASSERT_EQUAL(3 + 1, a);
	}

    void stub_a_method_with_mutable_lambda_delegate_always() {
        Mock<SomeInterface> mock;

        int mutableVar = 0;
        When(Method(mock, funcNoArgs)).AlwaysDo([mutableVar]() mutable {
            return ++mutableVar;
        });

        SomeInterface& i = mock.get();

        ASSERT_EQUAL(1, i.funcNoArgs());
        ASSERT_EQUAL(2, i.funcNoArgs());
    }

    static int func_delegate(int val) {
        return val;
    }

    static void proc_delegate(int val) {
        throw val;
    }

    void stub_a_method_with_static_method_delegate() {
        Mock<SomeInterface> mock;

        When(Method(mock, func)).Do(func_delegate);
        When(Method(mock, proc)).Do(proc_delegate);

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

        Method(mock, func) = [](int val) {
            return val;
        };
        Method(mock, proc) = [&a](int val) {
            a = val;
        };

        SomeInterface &i = mock.get();

        ASSERT_EQUAL(3, i.func(3));
        ASSERT_EQUAL(4, i.func(4));

        i.proc(3);
        i.proc(3);
        ASSERT_EQUAL(3, a);

		Method(mock, func) = [](int& val) {
			return val + 1;
		};
		Method(mock, proc) = [&a](int& val) {
			a = val + 1;
		};

		ASSERT_EQUAL(3 + 1, i.func(3));
		ASSERT_EQUAL(4 + 1, i.func(4));

		i.proc(3);
		i.proc(3);
		ASSERT_EQUAL(3 + 1, a);
	}

    void stub_only_specified_calls_by_assignment() {
        Mock<SomeInterface> mock;

        Fake(Method(mock, func));
        Fake(Method(mock, proc));

        Method(mock, func).Using(1) = [](...) {
            return 1;
        };
        Method(mock, proc).Using(1) = [](...) {
            throw std::string("error");
        };

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

        Method(mock, func) = func_delegate;
        Method(mock, proc) = proc_delegate;

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

        Method(mock, func) = 1;
        Method(mock, func).Using(2) = 2;

        SomeInterface &i = mock.get();

        ASSERT_EQUAL(1, i.func(3));
        ASSERT_EQUAL(1, i.func(4));
        ASSERT_EQUAL(2, i.func(2));
    }

    void stub_to_default_behavior_with_filter() {
        Mock<SomeInterface> mock;

        Fake(Method(mock, func).Using(1));
        Fake(Method(mock, proc).Using(1));

        SomeInterface &i = mock.get();

        ASSERT_EQUAL(0, i.func(1));
        ASSERT_THROW(i.func(2), fakeit::UnexpectedMethodCallException);

        i.proc(1);
        ASSERT_THROW(i.proc(2), fakeit::UnexpectedMethodCallException);
    }

    void change_method_behavior_with_filter() {
        class Exc : public std::exception {
        } e;

        Mock<SomeInterface> mock;
        When(Method(mock, func).Using(1)).Return(1);
        When(Method(mock, proc).Using(1)).Throw(e);

        SomeInterface &i = mock.get();

        ASSERT_EQUAL(1, i.func(1));
        ASSERT_THROW(i.func(2), fakeit::UnexpectedMethodCallException);

        ASSERT_THROW(i.proc(1), Exc);
        ASSERT_THROW(i.proc(2), fakeit::UnexpectedMethodCallException);
    }

    void change_method_behavior_with_functor_filter() {
        class Exc : public std::exception {
        } e;

        Mock<SomeInterface> mock;

        When(Method(mock, func)(1)).Return(1);
        When(Method(mock, proc)(1)).Throw(e);

        SomeInterface &i = mock.get();

        ASSERT_EQUAL(1, i.func(1));
        ASSERT_THROW(i.func(2), fakeit::UnexpectedMethodCallException);

        ASSERT_THROW(i.proc(1), Exc);
        ASSERT_THROW(i.proc(2), fakeit::UnexpectedMethodCallException);
    }

    void change_method_behavior_with_matcher() {
        class Exc : public std::exception {
        } e;

        Mock<SomeInterface> mock;

        When(Method(mock, func).Matching([](int a) {
            return a == 1;
        })).Return(1);
        When(Method(mock, proc).Matching([](int a) {
            return a == 1;
        })).Throw(e);

        SomeInterface &i = mock.get();

        ASSERT_EQUAL(1, i.func(1));
        ASSERT_THROW(i.func(2), fakeit::UnexpectedMethodCallException);

        ASSERT_THROW(i.proc(1), Exc);
        ASSERT_THROW(i.proc(2), fakeit::UnexpectedMethodCallException);
    }

    void change_method_behavior_with_functor_matcher() {
        class Exc : public std::exception {
        } e;

        Mock<SomeInterface> mock;

        When(Method(mock, func)([](int a) {
            return a == 1;
        })).Return(1);
        When(Method(mock, proc)([](int a) {
            return a == 1;
        })).Throw(e);

        SomeInterface &i = mock.get();

        ASSERT_EQUAL(1, i.func(1));
        ASSERT_THROW(i.func(2), fakeit::UnexpectedMethodCallException);

        ASSERT_THROW(i.proc(1), Exc);
        ASSERT_THROW(i.proc(2), fakeit::UnexpectedMethodCallException);
    }

	void change_method_behavior_with_by_ref_arguments_matcher() {
        class Exc : public std::exception {
        } e;

        Mock<SomeInterface> mock;

        When(Method(mock, func).Matching([](int& a) {
            return a == 1;
        })).Return(1);
        When(Method(mock, proc).Matching([](int& a) {
            return a == 1;
        })).Throw(e);

        SomeInterface &i = mock.get();

        ASSERT_EQUAL(1, i.func(1));
        ASSERT_THROW(i.func(2), fakeit::UnexpectedMethodCallException);

        ASSERT_THROW(i.proc(1), Exc);
        ASSERT_THROW(i.proc(2), fakeit::UnexpectedMethodCallException);
    }


    void stub_multiple_return_values() {
        Mock<SomeInterface> mock;

        When(Method(mock, func)).Return(1).Return(2);

        SomeInterface &i = mock.get();

        ASSERT_EQUAL(1, i.func(0));
        ASSERT_EQUAL(2, i.func(0));

        ASSERT_THROW(i.func(0), fakeit::UnexpectedMethodCallException);
        ASSERT_THROW(i.func(0), fakeit::UnexpectedMethodCallException);
    }

    void stub_multiple_return_values_using_queque() {
        Mock<SomeInterface> mock;
        std::queue<int> q({1, 2});

        When(Method(mock, func)).AlwaysDo([&](...) -> int {
            int rv = q.front();
            q.pop();
            return rv;
        });

        SomeInterface &i = mock.get();

        ASSERT_EQUAL(1, i.func(0));
        ASSERT_EQUAL(2, i.func(0));
    }

//
    void stub_multiple_throws() {

        Mock<SomeInterface> mock;

        When(Method(mock, func)).Throw(std::string("A")).Throw(std::string("B"));
        When(Method(mock, proc)).Throw(std::string("A")).Throw(std::string("B"));

        SomeInterface &i = mock.get();

        try {
            i.func(0);
        } catch (std::string &e) {
            ASSERT_EQUAL(std::string("A"), e);
        }

        try {
            i.func(0);
        } catch (std::string &e) {
            ASSERT_EQUAL(std::string("B"), e);
        }

        try {
            i.proc(0);
        } catch (std::string &e) {
            ASSERT_EQUAL(std::string("A"), e);
        }

        try {
            i.proc(0);
        } catch (std::string &e) {
            ASSERT_EQUAL(std::string("B"), e);
        }
    }

    void stub_multiple_return_values_with_list() {
        Mock<SomeInterface> mock;
        When(Method(mock, func)).Return(1, 2);
        SomeInterface &i = mock.get();

        ASSERT_EQUAL(1, i.func(1));
        ASSERT_EQUAL(2, i.func(1));
        ASSERT_THROW(i.func(1), fakeit::UnexpectedMethodCallException);
    }

    void stub_multiple_throws_with_list() {
        Mock<SomeInterface> mock;
        When(Method(mock, func)).Throw(std::string("1"), std::string("2"));
        When(Method(mock, proc)).Throw(std::string("1"), std::string("2"));
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
        When(Method(mock, func)).Do([](...) {
            return 1;
        }, [](...) {
            return 2;
        });
        When(Method(mock, proc)).Do([](...) {
        }, [](...) {
        });
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
                const auto &a = When(Method(mock, func));
                if (&a == &a) // use a to avoid unused variable compilation warning.
                    throw std::runtime_error("some exception");
            } catch (std::exception &) {
            }
        }
        SomeInterface &i = mock.get();
        ASSERT_THROW(i.func(1), fakeit::UnexpectedMethodCallException);
    }

    struct SomeInterfaceWithMember {
        virtual int func(int) = 0;

        std::string state;
    };

    void reset_mock_to_initial_state() {


        Mock<SomeInterfaceWithMember> mock;
        When(Method(mock, func)).AlwaysReturn(0);
        When(Method(mock, func).Using(1)).AlwaysReturn(1);
        mock.Stub(&SomeInterfaceWithMember::state, "state");
//
        SomeInterfaceWithMember&i = mock.get();
        i.func(0);
        i.func(1);

        Verify(Method(mock, func));
        Verify(Method(mock, func).Using(1));

        mock.Reset();

        ASSERT_THROW(i.func(0), fakeit::UnexpectedMethodCallException);
        ASSERT_THROW(i.func(1), fakeit::UnexpectedMethodCallException);

        ASSERT_THROW(Verify(Method(mock, func)), fakeit::VerificationException);
        ASSERT_THROW(Verify(Method(mock, func).Using(1)), fakeit::VerificationException);


        // stub agin and check it works again.
        When(Method(mock, func)).AlwaysReturn(0);
        When(Method(mock, func).Using(1)).AlwaysReturn(1);

        i.func(0);
        i.func(1);

        Verify(Method(mock, func));
        Verify(Method(mock, func).Using(1));
    }

    struct SomeClass {
        virtual int foo(int* x) = 0;
    };

    void use_lambda_to_change_ptr_value() {


        Mock<SomeClass> mock;

        When(Method(mock, foo)).AlwaysDo([](int *x) {
            (*x)++;
            return 0;
        });

        SomeClass &i = mock.get();
        int num = 0;
        ASSERT_EQUAL(0, i.foo(&num));
        ASSERT_EQUAL(1, num);
    }

    struct ApiInterface {
        virtual bool apiMethod(int a, int b, int& result) = 0;
    };
	void assingOutParamsWithLambda(){


        Mock<ApiInterface> mock;
        When(Method(mock, apiMethod)).AlwaysDo([](int a, int b, int& result) {
            result = a + b;
            return true;
        });

        int result;
        ASSERT_TRUE(mock.get().apiMethod(1,2,result));
        ASSERT_EQUAL(3,result);
    }

    void return_ref_to_lambda_member() {
        Mock<SomeInterface> mock;

        std::string str = "Some string with some content inside it";
        When(Method(mock, funcRetStrRef)).Do([str](int) -> const std::string& {return str;});

        SomeInterface& i = mock.get();

        ASSERT_EQUAL(i.funcRetStrRef(5), str);

        ASSERT_THROW(i.funcRetStrRef(5), fakeit::UnexpectedMethodCallException);
    }

} __BasicStubbing;

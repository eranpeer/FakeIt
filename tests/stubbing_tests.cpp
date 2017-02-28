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

struct BasicStubbing : tpunit::TestFixture {
    BasicStubbing() :
            tpunit::TestFixture(
                    //
                    TEST(BasicStubbing::calling_an_unstubbed_method_should_raise_UnmockedMethodCallException),//
                    TEST(BasicStubbing::stub_method_to_default_behaviore_will_always_return_the_default_behaviore),//
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
                    TEST(BasicStubbing::assingOutParamsWithLambda)
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


    void reset_mock_to_initial_state() {
        struct SomeInterface {
            virtual int func(int) = 0;

            std::string state;
        };

        Mock<SomeInterface> mock;
        When(Method(mock, func)).AlwaysReturn(0);
        When(Method(mock, func).Using(1)).AlwaysReturn(1);
        mock.Stub(&SomeInterface::state, "state");
//
        SomeInterface &i = mock.get();
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

    void use_lambda_to_change_ptr_value() {
        struct SomeClass {
            virtual int foo(int *x) = 0;
        };

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

    void assingOutParamsWithLambda(){
        struct ApiInterface {
            virtual bool apiMethod(int a, int b, int& result) = 0;
        };

        Mock<ApiInterface> mock;
        When(Method(mock, apiMethod)).AlwaysDo([](int a, int b, int& result) {
            result = a + b;
            return true;
        });

        int result;
        ASSERT_TRUE(mock.get().apiMethod(1,2,result));
        ASSERT_EQUAL(3,result);
    }

} __BasicStubbing;


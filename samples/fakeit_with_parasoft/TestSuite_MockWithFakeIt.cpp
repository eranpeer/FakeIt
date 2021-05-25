#include "cpptest.h"

/* CPPTEST_TEST_SUITE_CODE_BEGIN AdditionalIncludes */

#include "fakeit.hpp"

/// BDD-style wrappers that add reporting and support cleaner documentation of test code
#define SCENARIO( desc )  CPPTEST_MESSAGE( cpptestFormat( "Scenario: %s", desc) );
#define GIVEN( desc )     CPPTEST_MESSAGE( cpptestFormat( "Given: %s", desc) );
#define AND_GIVEN( desc ) CPPTEST_MESSAGE( cpptestFormat( "And given: %s", desc) );
#define WHEN( desc )      CPPTEST_MESSAGE( cpptestFormat( "When: %s", desc) );
#define AND_WHEN( desc )  CPPTEST_MESSAGE( cpptestFormat( "And when: %s", desc) );
#define THEN( desc )      CPPTEST_MESSAGE( cpptestFormat( "Then: %s", desc) );
#define AND_THEN( desc )  CPPTEST_MESSAGE( cpptestFormat( "And: %s", desc) );

/* CPPTEST_TEST_SUITE_CODE_END AdditionalIncludes */

/* A simple interface to be used for stubbing and mocking */
class SomeInterface {
public:
    virtual ~SomeInterface() = default;
    virtual int foo(int) = 0;
    virtual int bar(int,int) = 0;
};

/**
 * Add this TestSuite to an existing Parasoft project to test your FakeIt setup.
 * This TestSuite is designed to exercise the functionality of the FakeIt mocking
 * framework.  The tests are loosely taken from the QuickStart page:
 *   https://github.com/eranpeer/FakeIt/wiki/Quickstart
 *
 * These tests have been compiled using the IAR Arm compiler v8.22.2 and have been
 * executed on the IAR C-Spy Simulator.  In order to work with IAR, FakeIt requires
 * The IAR project to be configured with:
 *    1) RTTI Support Enabled
 *         In the IAR IDE, Project->Options->C/C++ Compiler->Language: Select
 *         C++ options->"Enable RTTI".  This removes the "--no_rtti" compiler option
 *         from the command line.
 *    2) Linker configured to keep C++ virtual functions
 *         In the IAR IDE, Project->Options->Linker->Optimizations: Unselect
 *         "Perform C++ Virtual Function Elimination" to disable this optimization.
 *         This can also be done at the command line with the --no_vfe linker option.
 *
 * The IAR Compiler is also unable to compile FakeIt when instrumented by Parasoft.
 * Turn off instrumentation for the TestSuite by:
 *         In the Parasoft IDE, Select file->rt-clk->Properties->Parasoft->C/C++test->
 *         Execution Settings: unselect "Instrument Code"
 *         Or add the following line similar to the .parasoft file:
 *         /<project>/tests/TestSuite_MockWithFakeIt.cpp//com.parasoft.xtest.execution.api.cpp.options.instrument.file=false
 *
 */
class TestSuite_MockWithFakeIt : public CppTest_TestSuite
{
    public:
        CPPTEST_TEST_SUITE(TestSuite_MockWithFakeIt);
        CPPTEST_TEST_SUITE_SETUP(testSuiteSetUp);
        CPPTEST_TEST(verifyStubForSomeInterface);
        CPPTEST_TEST(verifyMockForSomeInterface);
        CPPTEST_TEST_SUITE_TEARDOWN(testSuiteTearDown);
        CPPTEST_TEST(verifyStubForMultipleCallsOnInterface);
        CPPTEST_TEST(verifyFakeForSomeInterface);
        CPPTEST_TEST(verifyArgumentMatchingForSomeInterface);
        CPPTEST_TEST(verifyInvocationMatching);
        CPPTEST_TEST(verifySpyingOnClass);
        CPPTEST_TEST(verifyStubForOverloaded);
        CPPTEST_TEST(verifyMockForDefaultArguments);
        CPPTEST_TEST(verifyMockForSubclass);
        CPPTEST_TEST(verifyUsingSharedMock);
        CPPTEST_TEST(verifyUsingSharedMockAgain);
        CPPTEST_TEST(verifyMockForParameterReference);
        CPPTEST_TEST_SUITE_END();

        static void testSuiteSetUp();
        static void testSuiteTearDown();

        void setUp();
        void tearDown();

        void verifyStubForSomeInterface();
        void verifyMockForSomeInterface();
        void verifyStubForMultipleCallsOnInterface();
        void verifyFakeForSomeInterface();
        void verifyArgumentMatchingForSomeInterface();
        void verifyInvocationMatching();
        void verifySpyingOnClass();
        void verifyStubForOverloaded();
        void verifyMockForDefaultArguments();
        void verifyMockForSubclass();

		fakeit::Mock<SomeInterface> sharedMock;
        void verifyUsingSharedMock();
        void verifyUsingSharedMockAgain();
        void verifyMockForParameterReference();
};

CPPTEST_TEST_SUITE_REGISTRATION(TestSuite_MockWithFakeIt);

void TestSuite_MockWithFakeIt::testSuiteSetUp()
{
/* CPPTEST_TEST_SUITE_CODE_BEGIN TestSuiteSetUp */
/* CPPTEST_TEST_SUITE_CODE_END TestSuiteSetUp */
}

void TestSuite_MockWithFakeIt::testSuiteTearDown()
{
/* CPPTEST_TEST_SUITE_CODE_BEGIN TestSuiteTearDown */
/* CPPTEST_TEST_SUITE_CODE_END TestSuiteTearDown */
}

void TestSuite_MockWithFakeIt::setUp()
{
/* CPPTEST_TEST_SUITE_CODE_BEGIN TestCaseSetUp */
/* CPPTEST_TEST_SUITE_CODE_END TestCaseSetUp */
}

void TestSuite_MockWithFakeIt::tearDown()
{
	sharedMock.Reset();

	// To clear just the invocation history
	// sharedMock.ClearInvocationHistory();
}

/* CPPTEST_TEST_CASE_BEGIN verifyStubForSomeInterface */
void TestSuite_MockWithFakeIt::verifyStubForSomeInterface()
{
	using namespace fakeit;
	SCENARIO("Verify that methods can be stubbed for SomeInterface");
	{
		GIVEN("a Mock for SomeInterface");
		{
			Mock<SomeInterface> mock;
			WHEN("the 'foo' interface method returns 20 on first call");
			{
				When(Method(mock,foo)).Return(20);
				THEN("stub returns 20 on first call");
				{
					SomeInterface & i = mock.get();
					int result = i.foo(100);
					CPPTEST_ASSERT_INTEGER_EQUAL(20, result);
				}
			}
		}
	}
}
/* CPPTEST_TEST_CASE_END verifyStubForSomeInterface */

/* CPPTEST_TEST_CASE_BEGIN verifyStubForMultipleCallsOnInterface */
void TestSuite_MockWithFakeIt::verifyStubForMultipleCallsOnInterface()
{
	using namespace fakeit;
	SCENARIO("Verify multiple subsequent calls can be stubbed for SomeInterface");
	{
		GIVEN("a Mock for SomeInterface");
		{
			Mock<SomeInterface> mock;
			WHEN("the 'foo' interface method returns 1, 2, 3 for subsequent calls");
			{
				When(Method(mock,foo)).Return(1).Return(2).Return(3);
				THEN("stub returns 1, 2 and 3 when called");
				{
					SomeInterface & i = mock.get();
					int result = i.foo(100);
					CPPTEST_ASSERT_INTEGER_EQUAL(1, result);
					result = i.foo(100);
					CPPTEST_ASSERT_INTEGER_EQUAL(2, result);
					result = i.foo(100);
					CPPTEST_ASSERT_INTEGER_EQUAL(3, result);
				}
			}
		}
		GIVEN("another Mock for SomeInterface");
		{
			Mock<SomeInterface> mock;
			WHEN("the 'foo' interface method returns 10 N_Times(N=3) and 20 N_Times(N=4) for subsequent calls");
			{
				When(Method(mock,foo)).Return(3_Times(10), 4_Times(20));
				THEN("stub returns 10, 10, 10, 20, 20, 20, 20 when called");
				{
					SomeInterface & i = mock.get();
					int result = i.foo(100);
					CPPTEST_ASSERT_INTEGER_EQUAL(10, result);
					result = i.foo(100);
					CPPTEST_ASSERT_INTEGER_EQUAL(10, result);
					result = i.foo(100);
					CPPTEST_ASSERT_INTEGER_EQUAL(10, result);
					result = i.foo(100);
					CPPTEST_ASSERT_INTEGER_EQUAL(20, result);
					result = i.foo(100);
					CPPTEST_ASSERT_INTEGER_EQUAL(20, result);
					result = i.foo(100);
					CPPTEST_ASSERT_INTEGER_EQUAL(20, result);
					result = i.foo(100);
					CPPTEST_ASSERT_INTEGER_EQUAL(20, result);
				}
			}
		}
		GIVEN("yet another Mock for SomeInterface");
		{
			Mock<SomeInterface> mock;
			WHEN("the 'foo' interface method always returns 100 for all subsequent calls");
			{
				When(Method(mock,foo)).AlwaysReturn(100);
				THEN("stub returns 1, 2 and 3 when called");
				{
					SomeInterface & i = mock.get();
					int result = i.foo(100);
					CPPTEST_ASSERT_INTEGER_EQUAL(100, result);
					result = i.foo(100);
					CPPTEST_ASSERT_INTEGER_EQUAL(100, result);
				}
			}
		}
		GIVEN("and one last Mock for SomeInterface");
		{
			Mock<SomeInterface> mock;
			WHEN("the 'foo' interface method always returns 100 for 'foo(1)', else returns 0");
			{
				When(Method(mock,foo)).AlwaysReturn(0);
				When(Method(mock,foo).Using(1)).AlwaysReturn(100);
				THEN("stub returns 100 when called with '1', else returns 0");
				{
					SomeInterface & i = mock.get();
					int result = i.foo(100);
					CPPTEST_ASSERT_INTEGER_EQUAL(0, result);
					result = i.foo(1);
					CPPTEST_ASSERT_INTEGER_EQUAL(100, result);
					result = i.foo(2);
					CPPTEST_ASSERT_INTEGER_EQUAL(0, result);
					result = i.foo(3);
					CPPTEST_ASSERT_INTEGER_EQUAL(0, result);
					result = i.foo(4);
					CPPTEST_ASSERT_INTEGER_EQUAL(0, result);
					result = i.foo(1);
					CPPTEST_ASSERT_INTEGER_EQUAL(100, result);
				}
			}
		}
	}
	// Stub 'foo(1)' to always return '100'. For all other calls always return 0.

}
/* CPPTEST_TEST_CASE_END verifyStubForMultipleCallsOnInterface */

/* CPPTEST_TEST_CASE_BEGIN verifyMockForSomeInterface */
void TestSuite_MockWithFakeIt::verifyMockForSomeInterface()
{
	using namespace fakeit;
	SCENARIO("Verify that Mock object can be used to verify calls to SomeInterface methods");
	{
		GIVEN("a Mock for SomeInterface");
		{
			Mock<SomeInterface> mock;
			WHEN("the methods are faked and called");
			{
				Fake(Method(mock,foo));
				Fake(Method(mock,bar));
				SomeInterface & i = mock.get();
			    i.foo(1);
			    i.foo(2);
			    i.foo(3);
			    i.bar(2,1);
				THEN("Verify foo was invoked at least once. (The four lines do exactly the same)");
				{
				    Verify(Method(mock,foo));
				    Verify(Method(mock,foo)).AtLeastOnce();
				    Verify(Method(mock,foo)).AtLeast(1);
				    Verify(Method(mock,foo)).AtLeast(1_Time);
				}
				AND_THEN("Verify foo was invoked exactly 3 times. (The next two lines do exactly the same)");
				{
				    Verify(Method(mock,foo)).Exactly(3);
				    Verify(Method(mock,foo)).Exactly(3_Times);
				}
				AND_THEN("Verify foo(1) was invoked exactly once");
				{
				    Verify(Method(mock,foo).Using(1)).Once();
				    Verify(Method(mock,foo).Using(1)).Exactly(Once);
				}
				AND_THEN("Verify bar(a>b) was invoked exactly once");
				{
				    Verify(Method(mock,bar).Matching([](int a, int b){return a > b;})).Exactly(Once);
				    // Or, with C++14:
				    Verify(Method(mock,bar).Matching([](auto a, auto b){return a > b;})).Exactly(Once);
				}
				AND_THEN("Verify foo(1) was invoked anywhere before foo(3)");
				{
					Verify(Method(mock,foo).Using(1), Method(mock,foo).Using(3));
				}
				AND_THEN("Verify an invocation of foo() was followed by an invocation of bar()");
				{
					Verify(Method(mock,foo), Method(mock,bar));
				}
				AND_THEN("Verify no other invocation of any method of mock");
				{
					VerifyNoOtherInvocations(mock);
				}
			}
		}
	}
}
/* CPPTEST_TEST_CASE_END verifyMockForSomeInterface */


/* CPPTEST_TEST_CASE_BEGIN verifyFakeForSomeInterface */
void TestSuite_MockWithFakeIt::verifyFakeForSomeInterface()
{
	using namespace fakeit;
	SCENARIO("Verify that methods can be faked to return default values for SomeInterface");
	{
		GIVEN("a Mock for SomeInterface");
		{
			Mock<SomeInterface> mock;
			WHEN("the interface methods are faked");
			{
				Fake(Method(mock,foo));
				Fake(Method(mock,bar));
				THEN("all calls will return 0 (default value for return type)");
				{
					SomeInterface & i = mock.get();
					int result = i.foo(100);
					CPPTEST_ASSERT_INTEGER_EQUAL(0, result);
					result = i.bar(50, 100);
					CPPTEST_ASSERT_INTEGER_EQUAL(0, result);
				}
			}
		}
	}
}
/* CPPTEST_TEST_CASE_END verifyFakeForSomeInterface */

/* CPPTEST_TEST_CASE_BEGIN verifyArgumentMatchingForSomeInterface */
void TestSuite_MockWithFakeIt::verifyArgumentMatchingForSomeInterface()
{
	using namespace fakeit;
	SCENARIO("Verify argument matching for method calls on SomeInterface");
	{
		GIVEN("a Mock for SomeInterface");
		{
			Mock<SomeInterface> mock;
			WHEN("foo is set to return 100 for > 1");
			{
				When(Method(mock,foo)).AlwaysReturn(0);
				When(Method(mock,foo).Using(Gt(1))).AlwaysReturn(100);
				THEN("calls will return 0 or 100 based on argument");
				{
					SomeInterface & i = mock.get();
					int result = i.foo(-1);
					CPPTEST_ASSERT_INTEGER_EQUAL(0, result);
					result = i.foo(0);
					CPPTEST_ASSERT_INTEGER_EQUAL(0, result);
					result = i.foo(1);
					CPPTEST_ASSERT_INTEGER_EQUAL(0, result);
					result = i.foo(2);
					CPPTEST_ASSERT_INTEGER_EQUAL(100, result);
				}
			}
		}
		GIVEN("another Mock for SomeInterface");
		{
			Mock<SomeInterface> mock;
			WHEN("foo is set to return 99 for >= 10");
			{
				When(Method(mock,foo)).AlwaysReturn(0);
				When(Method(mock,foo).Using(Ge(10))).AlwaysReturn(99);
				THEN("calls will return 0 or 99 based on argument");
				{
					SomeInterface & i = mock.get();
					int result = i.foo(9);
					CPPTEST_ASSERT_INTEGER_EQUAL(0, result);
					result = i.foo(10);
					CPPTEST_ASSERT_INTEGER_EQUAL(99, result);
					result = i.foo(11);
					CPPTEST_ASSERT_INTEGER_EQUAL(99, result);
				}
			}
		}
		GIVEN("yet another Mock for SomeInterface");
		{
			Mock<SomeInterface> mock;
			WHEN("foo is set to return 1000 for < 200");
			{
				When(Method(mock,foo)).AlwaysReturn(10);
				When(Method(mock,foo).Using(Lt(200))).AlwaysReturn(1000);
				THEN("calls will return 10 or 1000 based on argument");
				{
					SomeInterface & i = mock.get();
					int result = i.foo(199);
					CPPTEST_ASSERT_INTEGER_EQUAL(1000, result);
					result = i.foo(200);
					CPPTEST_ASSERT_INTEGER_EQUAL(10, result);
					result = i.foo(201);
					CPPTEST_ASSERT_INTEGER_EQUAL(10, result);
				}
			}
		}
		GIVEN("yet another Mock for SomeInterface");
		{
			Mock<SomeInterface> mock;
			WHEN("foo is set to return 2000 for <= 300");
			{
				When(Method(mock,foo)).AlwaysReturn(55);
				When(Method(mock,foo).Using(Le(300))).AlwaysReturn(2000);
				THEN("calls will return 55 or 2000 based on argument");
				{
					SomeInterface & i = mock.get();
					int result = i.foo(299);
					CPPTEST_ASSERT_INTEGER_EQUAL(2000, result);
					result = i.foo(300);
					CPPTEST_ASSERT_INTEGER_EQUAL(2000, result);
					result = i.foo(301);
					CPPTEST_ASSERT_INTEGER_EQUAL(55, result);
				}
			}
		}
		GIVEN("one last Mock for SomeInterface");
		{
			Mock<SomeInterface> mock;
			WHEN("foo is set to return 5000 for != 500");
			{
				When(Method(mock,foo)).AlwaysReturn(555);
				When(Method(mock,foo).Using(Ne(500))).AlwaysReturn(5000);
				THEN("calls will return 555 or 5000 based on argument");
				{
					SomeInterface & i = mock.get();
					int result = i.foo(300);
					CPPTEST_ASSERT_INTEGER_EQUAL(5000, result);
					result = i.foo(3000);
					CPPTEST_ASSERT_INTEGER_EQUAL(5000, result);
					result = i.foo(500);
					CPPTEST_ASSERT_INTEGER_EQUAL(555, result);
				}
			}
		}
	}
}

/* CPPTEST_TEST_CASE_END verifyArgumentMatchingForSomeInterface */

/* CPPTEST_TEST_CASE_BEGIN verifyInvocationMatching */
void TestSuite_MockWithFakeIt::verifyInvocationMatching()
{
	using namespace fakeit;
	SCENARIO("Verify invocaton matching for method calls on SomeInterface");
	{
		GIVEN("a Mock for SomeInterface");
		{
			Mock<SomeInterface> mock;
			WHEN("bar is set to return 100 for a > b");
			{
				When(Method(mock,bar)).AlwaysReturn(10);
				When(Method(mock,bar).Matching([](auto a, auto b){return a > b;})).AlwaysReturn(100);
				THEN("calls will return 10 or 100 based on arguments");
				{
					SomeInterface & i = mock.get();
					int result = i.bar(1, 2);
					CPPTEST_ASSERT_INTEGER_EQUAL(10, result);
					result = i.bar(2, 2);
					CPPTEST_ASSERT_INTEGER_EQUAL(10, result);
					result = i.bar(200, 199);
					CPPTEST_ASSERT_INTEGER_EQUAL(100, result);
					result = i.bar(198, 199);
					CPPTEST_ASSERT_INTEGER_EQUAL(10, result);
					result = i.bar(110, 99);
					CPPTEST_ASSERT_INTEGER_EQUAL(100, result);
				}
			}
		}
	}
}
/* CPPTEST_TEST_CASE_END verifyInvocationMatching */

/* CPPTEST_TEST_CASE_BEGIN verifySpyingOnClass */
void TestSuite_MockWithFakeIt::verifySpyingOnClass()
{
	using namespace fakeit;
	SCENARIO("Verify that Mock object can be used to spy on a concrete class");
	{
		GIVEN("a concrete class and a mock object");
		{
			class SomeClass {
			public:
			   virtual int func1(int arg) {
			      return arg;
			   }
			   virtual int func2(int arg) {
			      return arg;
			   }
			};
			SomeClass obj;
			Mock<SomeClass> spy(obj);
			WHEN("the mock object is used to override func1 and to spy on func2");
			{
				When(Method(spy, func1)).AlwaysReturn(10); // Override to return 10
				Spy(Method(spy, func2)); // Spying func2 without changing any behavior
				THEN("func1 has overriden behavior and func2 has original behavior");
				{
					SomeClass& i = spy.get();
					int result = i.func1(1);
					CPPTEST_ASSERT_INTEGER_EQUAL(10, result);
					result = i.func2(1);
					CPPTEST_ASSERT_INTEGER_EQUAL(1, result);
				}
				AND_THEN("func1(1) and func2(1) were each invoked exactly once");
				{
				    Verify(Method(spy,func1).Using(1)).Once();
				    Verify(Method(spy,func2).Using(1)).Once();
				}
			}
		}
	}
}
/* CPPTEST_TEST_CASE_END verifySpyingOnClass */

/* CPPTEST_TEST_CASE_BEGIN verifyStubForOverloaded */
void TestSuite_MockWithFakeIt::verifyStubForOverloaded()
{
	using namespace fakeit;
	SCENARIO("Verify that stub object can be used handle overloaded function calls");
	{
		GIVEN("a stub for an interface with overloaded functions");
		{
			class SomeOverloadedInterface {
			public:
				virtual ~SomeOverloadedInterface() = default;
				virtual int func() = 0;
				virtual int func(int) = 0;
				virtual int func(int, std::string) = 0;
				virtual int func(int) const = 0;
			};

			Mock<SomeOverloadedInterface> mock;
			WHEN("the overloaded methods are stubbed to return different values");
			{
				When(OverloadedMethod(mock,func, int())).Return(1);
				When(OverloadedMethod(mock,func, int(int))).Return(2);
				When(OverloadedMethod(mock,func, int(int, std::string))).Return(3);
				When(ConstOverloadedMethod(mock,func, int(int))).Return(4);
				THEN("stub object will return the different values based on the overloaded arguments");
				{
					SomeOverloadedInterface& i = mock.get();
					int result = i.func();
					CPPTEST_ASSERT_INTEGER_EQUAL(1, result);
					result = i.func(1);
					CPPTEST_ASSERT_INTEGER_EQUAL(2, result);
					result = i.func(1, "");
					CPPTEST_ASSERT_INTEGER_EQUAL(3, result);
					const SomeOverloadedInterface& c = mock.get();
					result = c.func(1);
					CPPTEST_ASSERT_INTEGER_EQUAL(4, result);
				}
			}
		}
	}
}
/* CPPTEST_TEST_CASE_END verifyStubForOverloaded */

/* CPPTEST_TEST_CASE_BEGIN verifyMockForDefaultArguments */
void TestSuite_MockWithFakeIt::verifyMockForDefaultArguments()
{
	using namespace fakeit;
	SCENARIO("Verify that stub object can be used handle function calls with default arguments");
	{
		GIVEN("a stub for an interface with functions that have default parameters");
		{
			class InterfaceWithDefaults {
			public:
				virtual ~InterfaceWithDefaults() = default;
				virtual int funcDefaults(int val, int valDef=10) = 0;
			};

			Mock<InterfaceWithDefaults> mock;
			WHEN("the methods with default arguments are stubbed");
			{
				When(Method(mock,funcDefaults)).AlwaysReturn(1);
				THEN("stub object will allow method to be called with and without default parameters");
				{
					InterfaceWithDefaults& i = mock.get();
					int result = i.funcDefaults(5);
					CPPTEST_ASSERT_INTEGER_EQUAL(1, result);
					result = i.funcDefaults(1, 20);
					CPPTEST_ASSERT_INTEGER_EQUAL(1, result);
				}
			}
		}
	}
}
/* CPPTEST_TEST_CASE_END verifyMockForDefaultArguments */

/* CPPTEST_TEST_CASE_BEGIN verifyMockForSubclass */
void TestSuite_MockWithFakeIt::verifyMockForSubclass()
{
	using namespace fakeit;
	SCENARIO("Verify that Mock object can be used to verify calls to methods of inherited interface");
	{
		GIVEN("a Mock for inherited interface");
		{
			class SubSomeInterface : public SomeInterface {
			public:
				virtual ~SubSomeInterface() = default;
				virtual int func1() = 0;
				virtual int func2(int) = 0;
				virtual void func3() = 0;
			};

			Mock<SubSomeInterface> mock;
			WHEN("the methods are configured to always return properly");
			{
				When(Method(mock,foo)).AlwaysReturn(0);
				When(Method(mock,bar)).AlwaysReturn(1);
				When(Method(mock,func1)).AlwaysReturn(2);
				When(Method(mock,func2)).AlwaysReturn(3);
				When(Method(mock,func3)).Return();
				SubSomeInterface& i = mock.get();
				THEN("verify the base class methods are mocked properly");
				{
					int result = i.foo(5);
					CPPTEST_ASSERT_INTEGER_EQUAL(0, result);
				    Verify(Method(mock,foo)).Once();
					result = i.bar(6,7);
					CPPTEST_ASSERT_INTEGER_EQUAL(1, result);
				    Verify(Method(mock,bar)).Once();
				}
				AND_THEN("verify the subclass methods are mocked properly");
				{
					int result = i.func1();
					CPPTEST_ASSERT_INTEGER_EQUAL(2, result);
				    Verify(Method(mock,func1)).Once();
					result = i.func2(100);
					CPPTEST_ASSERT_INTEGER_EQUAL(3, result);
				    Verify(Method(mock,func2)).Once();
					i.func3();
				    Verify(Method(mock,func3)).Once();
				}
			}
		}
	}
}
/* CPPTEST_TEST_CASE_END verifyMockForSubclass */

/* CPPTEST_TEST_CASE_BEGIN verifyUsingSharedMock */
void TestSuite_MockWithFakeIt::verifyUsingSharedMock()
{
	using namespace fakeit;
	SCENARIO("Verify shared mock object can be used across test cases");
	{
		GIVEN("a shared Mock");
		{
			WHEN("the methods are configured to always return properly");
			{
				When(Method(sharedMock,foo)).AlwaysReturn(10);
				When(Method(sharedMock,bar)).AlwaysReturn(11);
				THEN("verify the mocked methods return the expected values and are called only once");
				{
					SomeInterface& i = sharedMock.get();
					int result = i.foo(5);
					CPPTEST_ASSERT_INTEGER_EQUAL(10, result);
				    Verify(Method(sharedMock,foo)).Once();
					result = i.bar(6,7);
					CPPTEST_ASSERT_INTEGER_EQUAL(11, result);
				    Verify(Method(sharedMock,bar)).Once();
				}
			}
		}
	}
}
/* CPPTEST_TEST_CASE_END verifyUsingSharedMock */

/* CPPTEST_TEST_CASE_BEGIN verifyUsingSharedMockAgain */
void TestSuite_MockWithFakeIt::verifyUsingSharedMockAgain()
{
	using namespace fakeit;
	SCENARIO("Verify shared mock object can be used across test cases again");
	{
		GIVEN("a shared Mock");
		{
			WHEN("the methods are configured to always return properly but with different values");
			{
				When(Method(sharedMock,foo)).AlwaysReturn(20);
				When(Method(sharedMock,bar)).AlwaysReturn(21);
				THEN("verify the mocked methods return the expected values and are called only once");
				{
					SomeInterface& i = sharedMock.get();
					int result = i.foo(50);
					CPPTEST_ASSERT_INTEGER_EQUAL(20, result);
				    Verify(Method(sharedMock,foo)).Once();
					result = i.bar(60,70);
					CPPTEST_ASSERT_INTEGER_EQUAL(21, result);
				    Verify(Method(sharedMock,bar)).Once();
				}
			}
		}
	}
}
/* CPPTEST_TEST_CASE_END verifyUsingSharedMockAgain */

/* CPPTEST_TEST_CASE_BEGIN verifyMockForParameterReference */
void TestSuite_MockWithFakeIt::verifyMockForParameterReference()
{
	/// Addressing https://github.com/eranpeer/FakeIt/wiki/Dangers-of-checking-arguments-passed-by-reference
	using namespace fakeit;
	SCENARIO("Verify that Mock object can be used to check parameters passed-by-reference");
	{
		GIVEN("a Mock for method with pass-by-reference parameter");
		{
			class ParamRefInterface {
			public:
				virtual ~ParamRefInterface() = default;
				virtual int func2(int &) = 0;
			};

			Mock<ParamRefInterface> mock;
			WHEN("the methods is configured to save arguments");
			{
				std::vector<int> values;
				When(Method(mock,func2)).AlwaysDo([&](int & i)
						{
							values.push_back(i);
							return 10;
						});
				auto &i = mock.get();
				THEN("verify the arguments are saved.");
				{
					{
						// call from context so that parameter passed in falls out of scope.
						int arg = 100;
						int result = i.func2(arg);
						arg = 150;
						result = i.func2(arg);
					}
				    Verify(Method(mock,func2)).Twice();
					CPPTEST_ASSERT_INTEGER_EQUAL(100, values[0]);
					CPPTEST_ASSERT_INTEGER_EQUAL(150, values[1]);
				}
			}
		}
	}
}
/* CPPTEST_TEST_CASE_END verifyMockForParameterReference */

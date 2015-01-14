/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include <string>
#include <iosfwd>
#include <stdexcept>
#include <tuple>

#include "tpunit++.hpp"
#include "fakeit.hpp"
#include "mockutils/Formatter.hpp"

using namespace fakeit;

struct DefaultEventFormatting: tpunit::TestFixture {
	DefaultEventFormatting() :
			TestFixture(
			//
			TEST(DefaultEventFormatting::format_UnmockedMethodCallEvent),
			TEST(DefaultEventFormatting::format_UnmatchedMethodCallEvent),
			TEST(DefaultEventFormatting::format_AnyArguments),
			TEST(DefaultEventFormatting::format_Exactly_Once),
			TEST(DefaultEventFormatting::format_Atleast_Once),
			TEST(DefaultEventFormatting::format_NoMoreInvocations_VerificationFailure),
			TEST(DefaultEventFormatting::format_UserDefinedMatcher_in_expected_pattern),
			TEST(DefaultEventFormatting::format_actual_arguments),
			TEST(DefaultEventFormatting::format_expected_arguments),
			TEST(DefaultEventFormatting::format_expected_concatenated_sequence),
			TEST(DefaultEventFormatting::format_expected_repeated_sequence),
			TEST(DefaultEventFormatting::format_complex_sequence)
			) //
	{
	}

	template <typename T> std::string to_string(T& val){
		std::stringstream stream;
		stream << val;
		return stream.str();
	}

	struct SomeInterface {
		virtual int func(int) = 0;
		virtual void proc(int) = 0;

		virtual int all_types(
			char, 
			bool, 
			int, 
			unsigned int, 
			long, 
			unsigned long, 
			//long long, 
			//unsigned long long, 
			double,
			long double
			) = 0;
	};

	void format_UnmockedMethodCallEvent() {
		Mock<SomeInterface> mock;
		SomeInterface &i = mock.get();
		try {
			i.func(1);
			FAIL();
		}
		catch (UnexpectedMethodCallException& e)
		{
			std::string expectedMsg{"Unexpected method invocation: unknown()\n"};
			expectedMsg +=	"  An unmocked method was invoked. All used virtual methods must be stubbed!";
			std::string actual{to_string(e)};
			ASSERT_EQUAL(expectedMsg, actual);
		}
	}

	void format_UnmatchedMethodCallEvent() {
		Mock<SomeInterface> mock;
		Fake(Method(mock,func).Using(3));
		SomeInterface &i = mock.get();
		try {
			i.func(1);
			FAIL();
		}
		catch (UnexpectedMethodCallException& e)
		{
			std::string expectedMsg{"Unexpected method invocation: mock.func(1)\n  Could not find Any recorded behavior to support this method call."};
			std::string actual{to_string(e)};
			ASSERT_EQUAL(expectedMsg, actual);
		}
	}

	void format_AnyArguments() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, func)).setFileInfo("test file",1,"test method").Exactly(Once);
		}
		catch (SequenceVerificationException& e) {
			std::string expectedMsg;
			expectedMsg += "test file:1: Verification error\n";
			expectedMsg += "Expected pattern: mock.func( Any arguments )\n";
			expectedMsg += "Expected matches: exactly 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actualMsg{to_string(e)};
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void format_Exactly_Once() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, func)).setFileInfo("test file",1,"test method").Exactly(Once);
		}
		catch (SequenceVerificationException& e) {
			std::string expectedMsg;
			expectedMsg += "test file:1: Verification error\n";
			expectedMsg += "Expected pattern: mock.func( Any arguments )\n";
			expectedMsg += "Expected matches: exactly 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";

			std::string actualMsg{to_string(e)};
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void format_Atleast_Once() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, func)).setFileInfo("test file",1,"test method").AtLeast(Once);
		}
		catch (SequenceVerificationException& e) {
			std::string expectedMsg;
			expectedMsg += "test file:1: Verification error\n";
			expectedMsg += "Expected pattern: mock.func( Any arguments )\n";
			expectedMsg += "Expected matches: at least 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";

			std::string actualMsg {to_string(e)};
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void format_NoMoreInvocations_VerificationFailure() {
		Mock<SomeInterface> mock;
		try {
			Fake(Method(mock, func));
			mock.get().func(1);
			mock.get().func(2);
			fakeit::Verify(Method(mock, func).Using(1)).setFileInfo("test file",1,"test method");
			fakeit::VerifyNoOtherInvocations(Method(mock, func)) //
				.setFileInfo("test file",1,"test method");
		}
		catch (NoMoreInvocationsVerificationException& e) {
			std::string expectedMsg;
			expectedMsg += "test file:1: Verification error\n";
			expectedMsg += "Expected no more invocations!! But the following unverified invocations were found:\n";
			expectedMsg += "  mock.func(2)";//

			std::string actualMsg{to_string(e)};//
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void format_UserDefinedMatcher_in_expected_pattern() {
		Mock<SomeInterface> mock;
		When(Method(mock, func)).Return(0);
		//SomeInterface &i = mock.get();
		try {
			fakeit::Verify(Method(mock, func).Matching([](...){return true; })) //
				.setFileInfo("test file",1,"test method").Exactly(2);
			FAIL();
		}
		catch (SequenceVerificationException& e)
		{
			std::string expectedMsg;
			expectedMsg += "test file:1: Verification error\n";
			expectedMsg += "Expected pattern: mock.func( user defined matcher )\n";
			expectedMsg += "Expected matches: exactly 2\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";

			std::string actualMsg {to_string(e)};
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void format_actual_arguments() {
		Mock<SomeInterface> mock;
		When(Method(mock, all_types)).Return(0);
		//SomeInterface &i = mock.get();
		try {
			mock().all_types('a', true, 1, 1, 1, 1, 0, 0);
			fakeit::Verify(Method(mock, all_types)) //
				.setFileInfo("test file",1,"test method").Exactly(2);
			FAIL();
		}
		catch (SequenceVerificationException& e)
		{
			std::string expectedMsg;
			expectedMsg += "test file:1: Verification error\n";
			expectedMsg += "Expected pattern: mock.all_types( Any arguments )\n";
			expectedMsg += "Expected matches: exactly 2\n";
			expectedMsg += "Actual matches  : 1\n";
			expectedMsg += "Actual sequence : total of 1 actual invocations:\n";
			//expectedMsg += "  mock.all_types(?, true, 1, 1, 1, 1, 1, 1, 1.000000, 1.000000)";
			expectedMsg += "  mock.all_types(?, true, 1, 1, 1, 1, 0, 0)";

			std::string actualMsg {to_string(e)};
			std::cout << actualMsg;
			ASSERT_EQUAL(expectedMsg, actualMsg);

		}
	}

	void format_expected_arguments() {
		Mock<SomeInterface> mock;
		When(Method(mock, all_types)).Return(0);
		//SomeInterface &i = mock.get();
		try {
			fakeit::Verify(Method(mock, all_types).Using('a', true, 1, 1, 1, 1, 0, 0))//
				.setFileInfo("test file", 1, "test method").Exactly(2);
			FAIL();
		}
		catch (SequenceVerificationException& e)
		{
			std::string expectedMsg;
			expectedMsg += "test file:1: Verification error\n";
			//expectedMsg += "Expected pattern: mock.all_types('a', true, 1, 1, 1, 1, 1.000000)\n";
			expectedMsg += "Expected pattern: mock.all_types(?, true, 1, 1, 1, 1, 0, 0)\n";
			expectedMsg += "Expected matches: exactly 2\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actual {to_string(e)};
			ASSERT_EQUAL(expectedMsg, actual);
		}
	}

	void format_expected_concatenated_sequence() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(
				Method(mock, func)(1) + Method(mock, proc)(2))//
				.setFileInfo("test file", 1, "test method");
			FAIL();
		}
		catch (SequenceVerificationException& e)
		{
			std::string expectedMsg;
			expectedMsg += "test file:1: Verification error\n";
			expectedMsg += "Expected pattern: mock.func(1) + mock.proc(2)\n";
			expectedMsg += "Expected matches: at least 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actual{ to_string(e) };
			ASSERT_EQUAL(expectedMsg, actual);
		}
	}

	void format_expected_repeated_sequence() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(
				Method(mock, func)(1) * 2)//
				.setFileInfo("test file", 1, "test method");
			FAIL();
		}
		catch (SequenceVerificationException& e)
		{
			std::string expectedMsg;
			expectedMsg += "test file:1: Verification error\n";
			expectedMsg += "Expected pattern: mock.func(1) * 2\n";
			expectedMsg += "Expected matches: at least 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actual{ to_string(e) };
			ASSERT_EQUAL(expectedMsg, actual);
		}
	}

	void format_complex_sequence() {
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(
				(Method(mock, func)(1) + Method(mock, proc)(1)) * 2)//
				.setFileInfo("test file", 1, "test method");
			FAIL();
		}
		catch (SequenceVerificationException& e)
		{
			std::string expectedMsg;
			expectedMsg += "test file:1: Verification error\n";
			expectedMsg += "Expected pattern: (mock.func(1) + mock.proc(1)) * 2\n";
			expectedMsg += "Expected matches: at least 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actual{ to_string(e) };
			ASSERT_EQUAL(expectedMsg, actual);
		}

		try {
			fakeit::Verify(
				(Method(mock, func)(1) * 2 * 2))//
				.setFileInfo("test file", 1, "test method");
			FAIL();
		}
		catch (SequenceVerificationException& e)
		{
			std::string expectedMsg;
			expectedMsg += "test file:1: Verification error\n";
			expectedMsg += "Expected pattern: (mock.func(1) * 2) * 2\n";
			expectedMsg += "Expected matches: at least 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : total of 0 actual invocations.";
			std::string actual{ to_string(e) };
			ASSERT_EQUAL(expectedMsg, actual);
		}

	}

} __DefaultEventFormatting;

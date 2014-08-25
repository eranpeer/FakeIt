/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include <string>
#include <iostream>
#include <stdexcept>
#include <tuple>

#include "tpunit++.hpp"
#include "fakeit.hpp"
#include "mockutils/Formatter.hpp"

using namespace fakeit;


struct DefaultErrorFormatting: tpunit::TestFixture {
	DefaultErrorFormatting() :
			tpunit::TestFixture(
			//
			TEST(DefaultErrorFormatting::parse_UnmockedMethodCallException),
			TEST(DefaultErrorFormatting::parse_UnmatchedMethodCallException),
			TEST(DefaultErrorFormatting::parse_AnyArguments),
			TEST(DefaultErrorFormatting::parse_Exactly_Once),
			TEST(DefaultErrorFormatting::parse_Atleast_Once),
			TEST(DefaultErrorFormatting::parse_NoMoreInvocations_VerificationFailure),
			TEST(DefaultErrorFormatting::parse_UserDefinedMatcher_in_expected_pattern),
			TEST(DefaultErrorFormatting::parse_actual_arguments)
			) //
	{
	}

	fakeit::DefaultErrorFormatter formatter;

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
			long long, 
			unsigned long long, 
			double, 
			long double) = 0;
	};

	void parse_UnmockedMethodCallException() {
		Mock<SomeInterface> mock;
		SomeInterface &i = mock.get();
		try {
			i.func(1);
			FAIL();
		}
		catch (UnexpectedMethodCallException& e)
		{
			std::string expectedMsg("UnexpectedMethodCallException: Unknown method invocation. All used virtual methods must be stubbed!");
			ASSERT_EQUAL(expectedMsg, to_string(e));
		}
	}

	void parse_UnmatchedMethodCallException() {
		Mock<SomeInterface> mock;
		Fake(Method(mock,func).Using(3));
		SomeInterface &i = mock.get();
		try {
			i.func(1);
			FAIL();
		}
		catch (UnexpectedMethodCallException& e)
		{
			std::string expectedMsg("UnexpectedMethodCallException: Could not find any recorded behavior to support this method call");
			ASSERT_EQUAL(expectedMsg, to_string(e));
		}
	}

	void parse_AnyArguments() {
		Mock<SomeInterface> mock;
		try {
			Verify(Method(mock, func)).Exactly(Once);
		}
		catch (SequenceVerificationException&) {
			std::string expectedMsg;
			expectedMsg += "test file:1: Verification error\n";
			expectedMsg += "Expected pattern: mock.func( any arguments )\n";
			expectedMsg += "Expected matches: exactly 2\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : no actual invocations";

//			std::string actualMsg = formatter.format(e);
//			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void parse_Exactly_Once() {
		Mock<SomeInterface> mock;
		try {
			Verify(Method(mock, func)).Exactly(Once);
		}
		catch (SequenceVerificationException& e) {
			std::string expectedMsg;
			expectedMsg += "test file:1: Verification error\n";
			expectedMsg += "Expected pattern: mock.func( any arguments )\n";
			expectedMsg += "Expected matches: exactly 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : no actual invocations";

//			std::string actualMsg = formatter.format(e);
//			ASSERT_EQUAL(expectedMsg, actualMsg);
			ASSERT_EQUAL(std::string{"VerificationException: expected exactly 1 invocations but was 0"}, e.what());

		}
	}

	void parse_Atleast_Once() {
		Mock<SomeInterface> mock;
		try {
			Verify(Method(mock, func)).AtLeast(Once);
		}
		catch (SequenceVerificationException& e) {
			std::string expectedMsg;
			expectedMsg += "test file:1: Verification error\n";
			expectedMsg += "Expected pattern: mock.func( any arguments )\n";
			expectedMsg += "Expected matches: at least 1\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : no actual invocations";

			ASSERT_EQUAL(std::string{"VerificationException: expected at least 1 invocations but was 0"}, e.what());

//			std::string actualMsg = formatter.format(e);
//			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void parse_NoMoreInvocations_VerificationFailure() {
		Mock<SomeInterface> mock;
		try {
			Fake(Method(mock, func));
			mock.get().func(1);
			mock.get().func(2);
			Verify(Method(mock, func).Using(1));
			fakeit::VerifyNoOtherInvocations(Method(mock, func)) //
				.setFileInfo("test file",1,"test method");
		}
		catch (NoMoreInvocationsVerificationException& e) {
			std::string expectedMsg;
			expectedMsg += "test file:1: Verification error\n";
			expectedMsg += "Expected no more invocations!! But the following unverified invocations were found:\n";
			expectedMsg += "  mock.func(2)";//

			std::string actualMsg = formatter.format(e);//
			ASSERT_EQUAL(expectedMsg, actualMsg);
			ASSERT_EQUAL(std::string{"VerificationException: expected no more invocations but found 1"}, e.what());
		}
	}


	void parse_UserDefinedMatcher_in_expected_pattern() {
		Mock<SomeInterface> mock;
		When(Method(mock, func)).Return(0);
		//SomeInterface &i = mock.get();
		try {
			fakeit::Verify(Method(mock, func).Matching([](...){return true; })) //
				.setFileInfo("test file",1,"test method").Exactly(2);
			FAIL();
		}
		catch (SequenceVerificationException&)
		{
			std::string expectedMsg;
			expectedMsg += "test file:1: Verification error\n";
			expectedMsg += "Expected pattern: mock.func( user defined matcher )\n";
			expectedMsg += "Expected matches: exactly 2\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : no actual invocations";

//			std::string actualMsg = formatter.format(e);
//			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void parse_actual_arguments() {
		Mock<SomeInterface> mock;
		When(Method(mock, all_types)).Return(0);
		//SomeInterface &i = mock.get();
		try {
			mock().all_types('a', true, 1, 1, 1, 1, 1, 1, 1, 1);
			fakeit::Verify(Method(mock, all_types)) //
				.setFileInfo("test file",1,"test method").Exactly(2);
			FAIL();
		}
		catch (SequenceVerificationException&)
		{
			std::string expectedMsg;
			expectedMsg += "test file:1: Verification error\n";
			expectedMsg += "Expected pattern: mock.all_types( any arguments )\n";
			expectedMsg += "Expected matches: exactly 2\n";
			expectedMsg += "Actual matches  : 1\n";
			expectedMsg += "Actual sequence :\n";
			expectedMsg += "  mock.all_types( 'a', true, 1, 1, 1, 1, 1, 1, 1.0, 1.0 )";

//			std::string actualMsg = formatter.format(e);
	//		ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void parse_expected_arguments() {
		Mock<SomeInterface> mock;
		When(Method(mock, all_types)).Return(0);
		//SomeInterface &i = mock.get();
		try {
			fakeit::Verify(Method(mock, all_types).Using('a', true, 1, 1, 1, 1, 1, 1, 1, 1))//
				.setFileInfo("test file", 1, "test method").Exactly(2);
			FAIL();
		}
		catch (SequenceVerificationException&)
		{
			std::string expectedMsg;
			expectedMsg += "test file:1: Verification error\n";
			expectedMsg += "Expected pattern: mock.all_types( 'a', true, 1, 1, 1, 1, 1, 1, 1.0, 1.0 )\n";
			expectedMsg += "Expected matches: exactly 2\n";
			expectedMsg += "Actual matches  : 0\n";
			expectedMsg += "Actual sequence : no actual invocations";
			//ASSERT_EQUAL(expectedMsg, to_string(e));
		}
	}

} __DefaultErrorFormatting;

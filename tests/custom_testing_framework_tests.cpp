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

struct CustomTestingFramework : tpunit::TestFixture {
	CustomTestingFramework() :
	tpunit::TestFixture(
	//
	TEST(CustomTestingFramework::handle_UnexpectedMethodCallEvent),
	TEST(CustomTestingFramework::handle_SequenceVerificationEvent),
	TEST(CustomTestingFramework::handle_NoMoreInvocationsVerificationEvent)
	) //
	{
	}

	class TestingFrameworkAdapter : public EventHandler {
	public:

		class AssertionException {
		public:
			AssertionException(std::string msg):_msg(msg){
			}
			std::string _msg;
		};

		virtual void handle(const UnexpectedMethodCallEvent& e) {
//			TestingFrameworkAdapter adapter;
//			DefaultFakeit2 df;
//			df.setTestingFrameworkAdapter(adapter);
//			df.clearTestingFrameworkAdapter();

			throw AssertionException("UnexpectedMethodCallEvent");
		}

		virtual void handle(const SequenceVerificationEvent& e) {
			throw AssertionException("SequenceVerificationEvent");
		}

		virtual void handle(const NoMoreInvocationsVerificationEvent& e) {
			throw AssertionException("NoMoreInvocationsVerificationEvent");
		}
	};

	template <typename T> std::string to_string(T& val){
		std::stringstream stream;
		stream << val;
		return stream.str();
	}

	struct SomeInterface {
		virtual int func(int) = 0;
	};

	class finally {
	private:
		std::function<void()> finallyClause;
		finally(const finally &);
		finally& operator=(const finally &);
	public:
		explicit finally(std::function<void()> f) :
			finallyClause(f) {
		}

		~finally() {
			finallyClause();
		}
	};

	static void setup() {
		static TestingFrameworkAdapter adapter;
		Fakeit.setTestingFrameworkAdapter(adapter);
	}

	static void teardown() {
		Fakeit.clearTestingFrameworkAdapter();
	}

	void handle_UnexpectedMethodCallEvent() {
		setup();
		finally onExit(teardown);
		Mock<SomeInterface> mock;
		SomeInterface &i = mock.get();
		try {
			i.func(1);
			FAIL();
		}
		catch (TestingFrameworkAdapter::AssertionException& e)
		{
			std::string expectedMsg{ "UnexpectedMethodCallEvent" };
			ASSERT_EQUAL(expectedMsg, e._msg);
		}
	}


	void handle_SequenceVerificationEvent() {
		setup();
		finally onExit(teardown);
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, func)).setFileInfo("test file", 1, "test method").Exactly(Once);
		}
		catch (TestingFrameworkAdapter::AssertionException& e) {
			std::string expectedMsg{ "SequenceVerificationEvent" };
			ASSERT_EQUAL(expectedMsg, e._msg);
		}
	}

	void handle_NoMoreInvocationsVerificationEvent() {
		setup();
		finally onExit(teardown);
		Mock<SomeInterface> mock;
		try {
			Fake(Method(mock, func));
			mock.get().func(1);
			mock.get().func(2);
			fakeit::Verify(Method(mock, func).Using(1)).setFileInfo("test file", 1, "test method");
			fakeit::VerifyNoOtherInvocations(Method(mock, func)) //
				.setFileInfo("test file", 1, "test method");

		}
		catch (TestingFrameworkAdapter::AssertionException& e) {
			std::string expectedMsg{ "NoMoreInvocationsVerificationEvent" };
			ASSERT_EQUAL(expectedMsg, e._msg);
		}
	}

} __CustomTestingFramework;

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

using namespace fakeit;

struct CustomEventFormatting : tpunit::TestFixture {
	CustomEventFormatting() :
	tpunit::TestFixture(
	//
	TEST(CustomEventFormatting::format_UnexpectedMethodCallEvent),
	TEST(CustomEventFormatting::format_SequenceVerificationEvent),
	TEST(CustomEventFormatting::format_NoMoreInvocationsVerificationEvent)
	) //
	{
	}

	class CustomEventFormatter : public EventFormatter {

		virtual std::string format(const fakeit::UnexpectedMethodCallEvent&) {
			return{ "UnexpectedMethodCallEvent" };
		}

		virtual std::string format(const fakeit::SequenceVerificationEvent&) {
			return{ "SequenceVerificationEvent" };
		}

		virtual std::string format(const fakeit::NoMoreInvocationsVerificationEvent&) {
			return{ "NoMoreInvocationsVerificationEvent" };
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

    std::string formatLineNumner(std::string file, int num){
#ifndef __GNUG__
        return file + std::string("(") + std::to_string(num) + std::string(")");
#else
        return file + std::string(":") + std::to_string(num);
#endif
    }

	static void setup() {
		static CustomEventFormatter formatter;
		Fakeit.setCustomEventFormatter(formatter);
	}

	static void teardown() {
		Fakeit.resetCustomEventFormatter();
	}

	void format_UnexpectedMethodCallEvent() {
		setup();
		finally onExit(teardown);
		Mock<SomeInterface> mock;
		SomeInterface &i = mock.get();
		try {
			i.func(1);
			FAIL();
		}
		catch (UnexpectedMethodCallException& e)
		{
			std::string expectedMsg{ "UnexpectedMethodCallEvent" };
			std::string actual{ to_string(e) };
			ASSERT_EQUAL(expectedMsg, actual);
		}
	}


	void format_SequenceVerificationEvent() {
		setup();
		finally onExit(teardown);
		Mock<SomeInterface> mock;
		try {
			fakeit::Verify(Method(mock, func)).setFileInfo("test file", 1, "test method").Exactly(Once);
		}
		catch (SequenceVerificationException& e) {
            std::string expectedMsg{ formatLineNumner("test file",1)  + ": SequenceVerificationEvent" };
			std::string actualMsg{ to_string(e) };
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

	void format_NoMoreInvocationsVerificationEvent() {
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
		catch (NoMoreInvocationsVerificationException& e) {
            std::string expectedMsg{ formatLineNumner("test file", 1) + ": NoMoreInvocationsVerificationEvent" };
			std::string actualMsg{ to_string(e) };
			ASSERT_EQUAL(expectedMsg, actualMsg);
		}
	}

} __CustomErrorFormatting;

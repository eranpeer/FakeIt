#pragma once

#include <ostream>

#include "fakeit/DefaultFakeit.hpp"
#include "fakeit/EventHandler.hpp"
#include <CppUnitTestAssert.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace fakeit {

	class MsTestAdapter : public EventHandler {
		EventFormatter& _formatter;
	public:

		virtual ~MsTestAdapter() = default;
		MsTestAdapter(EventFormatter& formatter) :_formatter(formatter) {}

		virtual void handle(const UnexpectedMethodCallEvent& e) override
		{
			auto formattedMessage = _formatter.format(e);
			std::wstring wFormattedMessage = to_wstring(formattedMessage);
			Assert::Fail(wFormattedMessage.c_str());
		}

		virtual void handle(const SequenceVerificationEvent& e) override
		{
			auto formattedMessage = _formatter.format(e);
			std::wstring wFormattedMessage = to_wstring(formattedMessage);
			//std::wstring wfile = to_wstring(e.file());
			//__LineInfo lineInfo(wfile.c_str(), e.callingMethod().c_str(), e.line());
			//Assert::Fail(wFormattedMessage.c_str(), &lineInfo);
			Assert::Fail(wFormattedMessage.c_str());
		}

		virtual void handle(const NoMoreInvocationsVerificationEvent& e) override
		{
			auto formattedMessage = _formatter.format(e);
			std::wstring wFormattedMessage = to_wstring(formattedMessage);
			//std::wstring wfile = to_wstring(e.file());
			//__LineInfo lineInfo(wfile.c_str(), e.callingMethod().c_str(), e.line());
			//Assert::Fail(wFormattedMessage.c_str(), &lineInfo);
			Assert::Fail(wFormattedMessage.c_str());
		}


		std::wstring to_wstring(const std::string string) {
			return std::wstring(string.begin(), string.end());
		}
	};

	class MsTestFakeit : public DefaultFakeit {

	public:
		virtual ~MsTestFakeit() = default;

		MsTestFakeit()
			: _formatter(), _tpunitAdapter(*this) {
		}

		static MsTestFakeit &getInstance() {
			static MsTestFakeit instance;
			return instance;
		}

	protected:

		fakeit::EventHandler &accessTestingFrameworkAdapter() override {
			return _tpunitAdapter;
		}

		EventFormatter &accessEventFormatter() override {
			return _formatter;
		}

	private:

		DefaultEventFormatter _formatter;
		MsTestAdapter _tpunitAdapter;

	};

}

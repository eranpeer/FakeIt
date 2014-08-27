/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef FakeitContext_h__
#define FakeitContext_h__

#include "fakeit/EventHandler.hpp"
#include "fakeit/ErrorFormatter.hpp"
namespace fakeit {

	struct FakeitContext : public EventHandler, public ErrorFormatter {

		void handle(const UnexpectedMethodCallEvent& e) {
			auto& eh = getEventHandler();
			eh.handle(e);
		}

		void handle(const SequenceVerificationEvent& e) {
			auto& eh = getEventHandler();
			eh.handle(e);
		}

		void handle(const NoMoreInvocationsVerificationEvent& e) {
			auto& eh = getEventHandler();
			eh.handle(e);
		}

		std::string format(const fakeit::UnexpectedMethodCallEvent& e) {
			auto& eh = getErrorFormatter();
			return eh.format(e);
		}

		std::string format(const fakeit::SequenceVerificationEvent& e) {
			auto& eh = getErrorFormatter();
			return eh.format(e);
		}

		std::string format(const fakeit::NoMoreInvocationsVerificationEvent& e)  {
			auto& eh = getErrorFormatter();
			return eh.format(e);
		}

	protected:
		virtual EventHandler& getEventHandler() = 0;
		virtual ErrorFormatter& getErrorFormatter() = 0;
	};

}

#endif //

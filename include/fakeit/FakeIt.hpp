/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef FakeIt_h__
#define FakeIt_h__

#include "fakeit/EventHandler.hpp"
#include "fakeit/ErrorFormatter.hpp"
namespace fakeit {

	struct FakeIt : public EventHandler, public ErrorFormatter {

		virtual ~FakeIt() = default;

		void handle(const UnexpectedMethodCallException& e) {
			auto& eh = getEventHandler();
			eh.handle(e);
		}

		void handle(const SequenceVerificationException& e) {
			auto& eh = getEventHandler();
			eh.handle(e);
		}

		void handle(const NoMoreInvocationsVerificationException& e) {
			auto& eh = getEventHandler();
			eh.handle(e);
		}

		std::string format(const fakeit::UnexpectedMethodCallException& e) {
			auto& eh = getErrorFormatter();
			return eh.format(e);
		}

		std::string format(const fakeit::SequenceVerificationException& e) {
			auto& eh = getErrorFormatter();
			return eh.format(e);
		}

		std::string format(const fakeit::NoMoreInvocationsVerificationException& e)  {
			auto& eh = getErrorFormatter();
			return eh.format(e);
		}

	protected:
		virtual EventHandler& getEventHandler() = 0;
		virtual ErrorFormatter& getErrorFormatter() = 0;
	};

}

#endif //

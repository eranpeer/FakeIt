/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef FakeitContext_h__
#define FakeitContext_h__
#include <vector>
#include "fakeit/EventHandler.hpp"
#include "fakeit/EventFormatter.hpp"

namespace fakeit {

	struct FakeitContext : private EventHandler, protected EventFormatter {

		void handle(const UnexpectedMethodCallEvent& e) {
			fireEvent(e);
			auto& eh = getTestingFrameworkAdapter();
			eh.handle(e);
		}

		void handle(const SequenceVerificationEvent& e) {
			fireEvent(e);
			auto& eh = getTestingFrameworkAdapter();
			eh.handle(e);
		}

		void handle(const NoMoreInvocationsVerificationEvent& e) {
			fireEvent(e);
			auto& eh = getTestingFrameworkAdapter();
			eh.handle(e);
		}

		std::string format(const fakeit::UnexpectedMethodCallEvent& e) {
			auto& eh = getEventFormatter();
			return eh.format(e);
		}

		std::string format(const fakeit::SequenceVerificationEvent& e) {
			auto& eh = getEventFormatter();
			return eh.format(e);
		}

		std::string format(const fakeit::NoMoreInvocationsVerificationEvent& e)  {
			auto& eh = getEventFormatter();
			return eh.format(e);
		}

		void addEventHandler(EventHandler& eventListener){
			_eventListeners.push_back(&eventListener);
		}

	protected:
		virtual EventHandler& getTestingFrameworkAdapter() = 0;
		virtual EventFormatter& getEventFormatter() = 0;
	private:
		std::vector<EventHandler*> _eventListeners;

		void fireEvent(const fakeit::NoMoreInvocationsVerificationEvent& evt){
			for (auto listener : _eventListeners)
				listener->handle(evt);
		}

		void fireEvent(const fakeit::UnexpectedMethodCallEvent& evt){
			for (auto listener : _eventListeners)
				listener->handle(evt);
		}

		void fireEvent(const fakeit::SequenceVerificationEvent& evt){
			for (auto listener : _eventListeners)
				listener->handle(evt);
		}

	};

}

#endif //

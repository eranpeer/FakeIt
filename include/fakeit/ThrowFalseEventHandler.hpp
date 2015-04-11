//
// Created by eran on 11/04/2015.
//

#ifndef THROWFALSEEVENTHANDLER_HPP
#define THROWFALSEEVENTHANDLER_HPP

#include "fakeit/EventHandler.hpp"

namespace fakeit {
	class ThrowFalseEventHandler : public VerificationEventHandler {

		void handle(const SequenceVerificationEvent &e) override {
			throw false;
		}

		void handle(const NoMoreInvocationsVerificationEvent &e) override {
			throw false;
		}
	};
}
#endif //THROWFALSEEVENTHANDLER_HPP

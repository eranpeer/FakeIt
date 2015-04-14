//
// Created by eran on 11/04/2015.
//
#pragma once

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

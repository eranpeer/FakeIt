//
// Created by eran on 11/04/2015.
//
#pragma once

#include "fakeit/EventHandler.hpp"

namespace fakeit {
    class ThrowFalseEventHandler : public VerificationEventHandler {

        void handle(const SequenceVerificationEvent &) override {
            throw false;
        }

        void handle(const NoMoreInvocationsVerificationEvent &) override {
            throw false;
        }
    };
}

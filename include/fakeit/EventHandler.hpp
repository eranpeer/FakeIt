/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include "fakeit/FakeitEvents.hpp"

namespace fakeit {

    struct VerificationEventHandler {
        virtual void handle(const SequenceVerificationEvent &e) = 0;

        virtual void handle(const NoMoreInvocationsVerificationEvent &e) = 0;
    };

    struct EventHandler : public VerificationEventHandler {
        using VerificationEventHandler::handle;

        virtual void handle(const UnexpectedMethodCallEvent &e) = 0;
    };

}

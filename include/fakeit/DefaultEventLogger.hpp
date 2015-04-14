/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include <iostream>
#include "fakeit/EventHandler.hpp"
#include "fakeit/DefaultEventFormatter.hpp"
#include "fakeit/FakeitExceptions.hpp"

namespace fakeit {

    struct DefaultEventLogger : public fakeit::EventHandler {

        DefaultEventLogger(EventFormatter &formatter) : _formatter(formatter), _out(std::cout) { }

        virtual void handle(const UnexpectedMethodCallEvent &e) override {
            _out << _formatter.format(e) << std::endl;
        }

        virtual void handle(const SequenceVerificationEvent &e) override {
            _out << _formatter.format(e) << std::endl;
        }

        virtual void handle(const NoMoreInvocationsVerificationEvent &e) override {
            _out << _formatter.format(e) << std::endl;
        }

    private:
        EventFormatter &_formatter;
        std::ostream &_out;
    };

}

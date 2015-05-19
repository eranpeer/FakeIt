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
    
    struct FakeitException {
        std::exception err;

        virtual ~FakeitException() = default;

        virtual std::string what() const = 0;

        friend std::ostream &operator<<(std::ostream &os, const FakeitException &val) {
            os << val.what();
            return os;
        }
    };


    // Unlike verification exceptions (that are thrown from the test level), this exception 
    // should not inherit from std::exception so that production code will not catch it.
    struct UnexpectedMethodCallException : public FakeitException {

        UnexpectedMethodCallException(std::string format) :
                _format(format) {
        }

        virtual std::string what() const override {
            return _format;
        }

    private:
        std::string _format;
    };

}

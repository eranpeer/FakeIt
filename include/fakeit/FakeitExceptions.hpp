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

        virtual ~FakeitException() = default;

        virtual std::string what() const = 0;

        friend std::ostream &operator<<(std::ostream &os, const FakeitException &val) {
            os << val.what();
            return os;
        }
    };


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

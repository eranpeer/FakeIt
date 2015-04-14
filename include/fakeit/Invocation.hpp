/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include <typeinfo>
#include <tuple>
#include <string>
#include <iosfwd>
#include <sstream>

#include "fakeit/DomainObjects.hpp"
#include "mockutils/Destructible.hpp"

namespace fakeit {

    struct Invocation : public Destructible {

        static unsigned int nextInvocationOrdinal() {
            static std::atomic_uint invocationOrdinal{0};
            return ++invocationOrdinal;
        }

        struct Matcher {

            virtual ~Matcher() THROWS {
            }

            virtual bool matches(Invocation &invocation) = 0;

            virtual std::string format() const = 0;
        };

        Invocation(unsigned int ordinal, MethodInfo &method) :
                _ordinal(ordinal), _method(method), _isVerified(false) {
        }

        virtual ~Invocation() = default;

        unsigned int getOrdinal() const {
            return _ordinal;
        }

        MethodInfo &getMethod() const {
            return _method;
        }

        void markAsVerified() {
            _isVerified = true;
        }

        bool isVerified() const {
            return _isVerified;
        }

        virtual std::string format() const = 0;

    private:
        const unsigned int _ordinal;
        MethodInfo &_method;
        bool _isVerified;
    };

}
/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#pragma once

#include <typeinfo>
#include <unordered_set>
#include <tuple>
#include <string>
#include <iosfwd>
#include <atomic>
#include "mockutils/type_utils.hpp"


#include "mockutils/Macros.hpp"
#include "fakeit/Invocation.hpp"
#include "mockutils/TuplePrinter.hpp"


namespace fakeit {

    template<typename ... arglist>
    struct ActualInvocation : public Invocation {

        struct Matcher : public virtual Destructible {
            virtual bool matches(ActualInvocation<arglist...> &actualInvocation) = 0;

            virtual std::string format() const = 0;
        };

        ActualInvocation(unsigned int ordinal, MethodInfo &method, const typename fakeit::production_arg<arglist>::type... args) :
            Invocation(ordinal, method), _matcher{ nullptr }
            , actualArguments{ std::forward<arglist>(args)... }
        {
        }

        ArgumentsTuple<arglist...> & getActualArguments() {
            return actualArguments;
        }

        /**
         * The Matcher that was use to match this ActualInvocation.
         */
        void setActualMatcher(Matcher *matcher) {
            this->_matcher = matcher;
        }

        Matcher *getActualMatcher() {
            return _matcher;
        }

        std::string format() const override {
            std::ostringstream out;
            out << getMethod().name();
            print(out, actualArguments);
            return out.str();
        }

    private:

        Matcher *_matcher;
        ArgumentsTuple<arglist...> actualArguments;
    };

    template<typename ... arglist>
    std::ostream &operator<<(std::ostream &strm, const ActualInvocation<arglist...> &ai) {
        strm << ai.format();
        return strm;
    }

}

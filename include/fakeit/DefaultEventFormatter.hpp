/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on August, 2014
 */
#pragma once

#include <iosfwd>
#include "fakeit/EventFormatter.hpp"
#include "fakeit/FakeitEvents.hpp"

namespace fakeit {

    struct DefaultEventFormatter : public EventFormatter {

        virtual std::string format(const UnexpectedMethodCallEvent &e) override {
            std::ostringstream out;
            out << "Unexpected method invocation: ";
            out << e.getInvocation().format() << std::endl;
            if (UnexpectedType::Unmatched == e.getUnexpectedType()) {
                out << "  Could not find any recorded behavior to support this method call.";
            } else {
                out << "  An unmocked method was invoked. All used virtual methods must be stubbed!";
            }
            return out.str();
        }

        /*
         test file:1: Verification error\n
         Expected pattern: mock.all_types( 'a', true, 1, 1, 1, 1, 1, 1, 1.0, 1.0 )
         Expected matches: exactly 2
         Actual matches  : 0
         Actual sequence : no actual invocations
         */
        virtual std::string format(const SequenceVerificationEvent &e) override {
            std::ostringstream out;
            out << "Verification error" << std::endl;

            out << "Expected pattern: ";
            const std::vector<fakeit::Sequence *> expectedPattern = e.expectedPattern();
            out << formatExpectedPattern(expectedPattern) << std::endl;

            out << "Expected matches: ";
            formatExpectedCount(out, e.verificationType(), e.expectedCount());
            out << std::endl;

            out << "Actual matches  : " << e.actualCount() << std::endl;

            auto actualSequence = e.actualSequence();
            out << "Actual sequence : total of " << actualSequence.size() << " actual invocations";
            if (actualSequence.size() == 0) {
                out << ".";
            } else {
                out << ":" << std::endl;
            }
            formatInvocationList(out, actualSequence);

            return out.str();
        }

        virtual std::string format(const NoMoreInvocationsVerificationEvent &e) override {
            std::ostringstream out;
            out << "Verification error" << std::endl;
            out << "Expected no more invocations!! but the following unverified invocations were found:" << std::endl;
            formatInvocationList(out, e.unverifedIvocations());
            return out.str();
        }

        static std::string formatExpectedPattern(const std::vector<fakeit::Sequence *> &expectedPattern) {
            std::string expectedPatternStr;
            for (unsigned int i = 0; i < expectedPattern.size(); i++) {
                Sequence *s = expectedPattern[i];
                expectedPatternStr += formatSequence(*s);
                if (i < expectedPattern.size() - 1)
                    expectedPatternStr += " ... ";
            }
            return expectedPatternStr;
        }

    private:

        static std::string formatSequence(const Sequence &val) {
            const ConcatenatedSequence *cs = dynamic_cast<const ConcatenatedSequence *>(&val);
            if (cs) {
                return format(*cs);
            }
            const RepeatedSequence *rs = dynamic_cast<const RepeatedSequence *>(&val);
            if (rs) {
                return format(*rs);
            }

            // This is a leaf sequence. It has exactly one matcher! Format this matcher.
            std::vector<Invocation::Matcher *> vec;
            val.getExpectedSequence(vec);
            return vec[0]->format();
        }

        static void formatExpectedCount(std::ostream &out, fakeit::VerificationType verificationType,
                                        int expectedCount) {
            if (verificationType == fakeit::VerificationType::Exact)
                out << "exactly ";

            if (verificationType == fakeit::VerificationType::AtLeast)
                out << "at least ";

            out << expectedCount;
        }

        static void formatInvocationList(std::ostream &out, const std::vector<fakeit::Invocation *> &actualSequence) {
            size_t max_size = actualSequence.size();
            if (max_size > 50)
                max_size = 50;

            for (unsigned int i = 0; i < max_size; i++) {
                out << "  ";
                auto invocation = actualSequence[i];
                out << invocation->format();
                if (i < max_size - 1)
                    out << std::endl;
            }

            if (actualSequence.size() > max_size)
                out << std::endl << "  ...";
        }

        static std::string format(const ConcatenatedSequence &val) {
            std::ostringstream out;
            out << formatSequence(val.getLeft()) << " + " << formatSequence(val.getRight());
            return out.str();
        }

        static std::string format(const RepeatedSequence &val) {
            std::ostringstream out;
            const ConcatenatedSequence *cs = dynamic_cast<const ConcatenatedSequence *>(&val.getSequence());
            const RepeatedSequence *rs = dynamic_cast<const RepeatedSequence *>(&val.getSequence());
            if (rs || cs)
                out << '(';
            out << formatSequence(val.getSequence());
            if (rs || cs)
                out << ')';

            out << " * " << val.getTimes();
            return out.str();
        }
    };
}

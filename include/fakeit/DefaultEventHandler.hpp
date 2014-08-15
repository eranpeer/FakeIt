/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#ifndef DefaultEventHandler_h__
#define DefaultEventHandler_h__

#include <iostream>
#include "fakeit/EventHandler.hpp"

namespace fakeit {

struct DefaultErrorFormatter {

	virtual std::string format(UnexpectedMethodCallException& e)  {
		std::ostringstream out;
		out << "Unexpected Method Call: " << e.getInvocation().format();
		return out.str();
	}

	virtual std::string format(SequenceVerificationException& e) {
		std::ostringstream out;
		out << e.file() << ":" << e.line() << ": ";
		out << "VerificationException" << std::endl;

		out << "Expected ";
		formatExpectedCount(out, e.verificationType(), e.expectedCount());

		if (e.expectedPattern().size() == 1 && e.expectedPattern()[0]->size() == 1) {
			out << " of method: ";
		}
		else {
			out << " of pattern:" << std::endl;
		}

		for (unsigned int i = 0; i < e.expectedPattern().size(); i++) {
			Sequence* s = e.expectedPattern()[i];
			out << formatSequence(*s);
			if (i < e.expectedPattern().size() - 1)
				out << " ... ";
		}

		out << std::endl << "Actual sequence: ";
		auto actualSequence = e.actualSequence();
		if (actualSequence.size() == 0)
			out << "empty";
		if (actualSequence.size() > 1)
			out << std::endl;

		formatInvocationList(out, actualSequence);

		return out.str();
	}

	virtual std::string format(NoMoreInvocationsVerificationException& e) {
		std::ostringstream out;
		out << e.file() << ":" << e.line() << ": ";
		out << "Expected no more invocations!! But the following unverified invocations were found: " << std::endl;
		formatInvocationList(out, e.unverifedIvocations());
		return out.str();
	}

private:

	static std::string formatSequence(const Sequence& val) {
		const ConcatenatedSequence* cs = dynamic_cast<const ConcatenatedSequence*>(&val);
		if (cs) {
			return Formatter<ConcatenatedSequence>::format(*cs);
		}
		const RepeatedSequence* rs = dynamic_cast<const RepeatedSequence*>(&val);
		if (rs) {
			return Formatter<RepeatedSequence>::format(*rs);
		}

		// This is a leaf sequence. It has exactly one matcher! Format this matcher.
		std::vector<Invocation::Matcher*> vec;
		val.getExpectedSequence(vec);
		return vec[0]->format();
	}

	static void formatExpectedCount(std::ostream& out, fakeit::VerificationType verificationType, int expectedCount) {
		if (verificationType == fakeit::VerificationType::Exact)
			out << "exactly ";

		if (verificationType == fakeit::VerificationType::AtLeast)
			out << "at least ";

		if (expectedCount == 1)
			out << "one appearance";
		else {
			out << expectedCount << " appearances";
		}
	}

	static void formatInvocationList(std::ostream& out, const std::vector<fakeit::Invocation*>& actualSequence) {
		unsigned int max_size = actualSequence.size();
		if (max_size > 5)
			max_size = 5;

		for (unsigned int i = 0; i < max_size; i++) {
			auto invocation = actualSequence[i];
			out << invocation->format();
			if (i < max_size - 1)
				out << std::endl;
		}

		if (actualSequence.size() > max_size)
			out << std::endl << "...";
	}

	static std::string format(const ConcatenatedSequence& val) {
		std::ostringstream out;
		out << formatSequence(val.getLeft()) << " + " << formatSequence(val.getRight());
		return out.str();
	}

	static std::string format(const RepeatedSequence& val) {
		std::ostringstream out;
		out << formatSequence(val.getSequence()) << " * " << val.getTimes();
		return out.str();
	}
};

struct DefaultEventHandler: public fakeit::EventHandler {

	virtual void handle(UnexpectedMethodCallException& e) override {
		out << formatter.format(e) << std::endl;
	}

	virtual void handle(SequenceVerificationException& e) override {
		out << formatter.format(e) << std::endl;
	}

	virtual void handle(NoMoreInvocationsVerificationException& e) override {
		out << formatter.format(e) << std::endl;
	}

	DefaultEventHandler() :
			out(std::cout) {
	}
private:
	DefaultErrorFormatter formatter;
	std::ostream& out;
};

}

#endif

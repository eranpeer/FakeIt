/*
 * DefaultErrorFormatter.hpppp
 *
 *  Created on: Jan 29, 2014
 *      Author: eran
 */
#ifndef DefaultErrorFormatter_h__
#define DefaultErrorFormatter_h__

#include <vector>
#include "fakeit/ErrorFormatter.hpp"

namespace fakeit {

//struct DefaultErrorFormatter: public virtual ErrorFormatter {
//	virtual ~DefaultErrorFormatter() = default;
//
//	virtual std::string buildNoOtherInvocationsVerificationErrorMsg( //
//			std::vector<AnyInvocation*>& allIvocations, //
//			std::vector<AnyInvocation*>& unverifedIvocations) override {
//
//		auto format = std::string("found ") + std::to_string(unverifedIvocations.size()) + " non verified invocations.\n";
//		return format + toString(unverifedIvocations);
//	}
//
//	virtual std::string buildExactVerificationErrorMsg(
//			std::vector<Sequence*>& expectedPattern,
//			std::vector<AnyInvocation*>& actualSequence,
//			int expectedInvocationCount, int count) override {
//		return std::string("expected ") + std::to_string(expectedInvocationCount) + " but was " + std::to_string(count);
//	}
//
//	virtual std::string buildAtLeastVerificationErrorMsg(
//			std::vector<Sequence*>& expectedPattern,
//			std::vector<AnyInvocation*>& actualSequence,
//			int expectedInvocationCount, int count) override {
//		return std::string("Expected invocation sequence could not be found in actual invocation order");
//	}
//private:
//private:
//	std::string toString(std::vector<AnyInvocation*>& unverifedIvocations) {
//		std::string formattedString;
//		for (auto invocation : unverifedIvocations) {
//			formattedString += invocation->format();
//			formattedString += '\n';
//		}
//		return formattedString;
//	}
//}static defaultErrorFormatter;

}
#endif /* DefaultErrorFormatter_h__ */

#ifndef ErrorFormatter_h__
#define ErrorFormatter_h__

#include <vector>
#include "mock4cpp/ActualInvocation.h"

namespace mock4cpp {
struct ErrorFormatter {
	virtual ~ErrorFormatter() = default;

	virtual std::string buildNoOtherInvocationsVerificationErrorMsg( //
			std::vector<AnyInvocation*>& allIvocations, //
			std::vector<AnyInvocation*>& unverifedIvocations) = 0;

	virtual std::string buildExactVerificationErrorMsg(std::vector<Sequence*>& expectedPattern, std::vector<AnyInvocation*>& actualSequence,
			int expectedInvocationCount, int count)= 0;

	virtual std::string buildAtLeastVerificationErrorMsg(std::vector<Sequence*>& expectedPattern,
			std::vector<AnyInvocation*>& actualSequence, int expectedInvocationCount, int count)= 0;
};
}

#endif //ErrorFormatter_h__

#ifndef MockRepository_h__
#define MockRepository_h__

#include <vector>
#include "ActualInvocationSchenario.h"

namespace mock4cpp {

	struct MockBase {
		void addActualInvocation(ActualInvocation * invocation){
			invocations.push_back(invocation);
		}
	private:
		std::vector<ActualInvocation*> invocations;
	};
}

#endif // MockRepository_h__
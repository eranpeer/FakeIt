#ifndef MockRepository_h__
#define MockRepository_h__

#include <string>

namespace mock4cpp {

	struct MockObject {
	};

	struct Method {
		virtual ~Method() = default;
		virtual std::string getMethodName() const = 0;
		virtual MockObject& getMockObject() = 0;
	};

}

#endif // MockRepository_h__

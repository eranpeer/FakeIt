#ifndef DomainObjects_h__
#define DomainObjects_h__

#include <string>

namespace fakeit {

struct MockObject {
};

struct Method {
	virtual ~Method() = default;
	//virtual std::string getMethodName() const = 0;
};

}

#endif // DomainObjects_h__

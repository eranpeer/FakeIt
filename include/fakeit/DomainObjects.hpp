#ifndef DomainObjects_h__
#define DomainObjects_h__

#include <string>

namespace fakeit {

struct MockObject {
};

struct Method {
	virtual ~Method() = default;
};

}

#endif // DomainObjects_h__

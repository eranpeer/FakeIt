#ifndef UnmockedMethodCallException_h__
#define UnmockedMethodCallException_h__

#include <functional>
namespace mock4cpp {

	struct UnmockedMethodCallException : public std::exception {
		UnmockedMethodCallException() : std::exception("Could not find recorded implementation to support method call") {}
	};

}

#endif // UnmockedMethodCallException_h__

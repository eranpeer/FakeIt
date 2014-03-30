#ifndef FakeitExceptions_h__
#define FakeitExceptions_h__

#include <functional>
namespace fakeit {

struct UnexpectedMethodCallException: public std::exception {
	UnexpectedMethodCallException() {
	}

	const char* what() const throw () override {
		return "Unexpected method call. Could not find recorded implementation to support method call";
	}
};

struct VerificationException: public std::exception {
	VerificationException(const std::string& s) :
			std::exception(), msg { s } {
	}

	const char* what() const throw () override {
		return msg.c_str();
	}

private:
	const std::string msg;
};

}

#endif // FakeitExceptions_h__

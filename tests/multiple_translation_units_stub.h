#pragma once

#include <string>

#include "fakeit.hpp"

namespace multiple_tu {

	struct SomeInterface {
		virtual int func(int) = 0;
		virtual std::string func2(int) = 0;
	};

	void stubFunc(fakeit::Mock<SomeInterface>& mock);
	void stubFunc2(fakeit::Mock<SomeInterface>& mock);
	void stubMoreFunc(fakeit::Mock<SomeInterface>& mock);
	void stubMoreFunc2(fakeit::Mock<SomeInterface>& mock);

}

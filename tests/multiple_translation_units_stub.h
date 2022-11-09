#pragma once

#include <string>

#include "fakeit.hpp"

struct SomeInterface {
	virtual int func() = 0;
	virtual std::string func2() = 0;
};

void stubFunc2(fakeit::Mock<SomeInterface>& mock);
void stubFunc(fakeit::Mock<SomeInterface>& mock);

#include "multiple_translation_units_stub.h"


void stubFunc2(fakeit::Mock<SomeInterface>& mock)
{
	fakeit::When(Method(mock, func2)).AlwaysReturn<std::string>("String");
}

void stubFunc(fakeit::Mock<SomeInterface>& mock)
{
	fakeit::When(Method(mock, func)).AlwaysReturn<int>(3);
}

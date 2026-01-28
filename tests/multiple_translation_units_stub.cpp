#include "multiple_translation_units_stub.h"

namespace multiple_tu {

	void stubFunc(fakeit::Mock<SomeInterface>& mock)
	{
		fakeit::When(Method(mock, func)).Return(5);
	}

	void stubFunc2(fakeit::Mock<SomeInterface>& mock)
	{
		fakeit::When(Method(mock, func2)).Return("String");
	}

	void stubMoreFunc(fakeit::Mock<SomeInterface>& mock)
	{
		fakeit::When(Method(mock, func).Using(1)).Return(10);
		fakeit::When(Method(mock, func).Using(2)).Return(20);
	}

	void stubMoreFunc2(fakeit::Mock<SomeInterface>& mock)
	{
		fakeit::When(Method(mock, func2).Using(1)).Return("String1");
		fakeit::When(Method(mock, func2).Using(2)).Return("String2");
	}

}

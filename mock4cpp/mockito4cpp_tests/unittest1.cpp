#include "stdafx.h"
#include "CppUnitTest.h"
#include "mockito.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace mockito4cpp_tests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		struct SomeMethods {
			virtual bool boolFunc() = 0;
		};

		TEST_METHOD(TestMethod1)
		{
			Mock<SomeMethods> mock;
			mock.Stub(&SomeMethods::boolFunc);
			//When(i.boolFunc()).Retutn(true);
			// TODO: Your test code here
		}

	};
}
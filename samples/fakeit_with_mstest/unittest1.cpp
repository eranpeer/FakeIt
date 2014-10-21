#include "stdafx.h"
#include "CppUnitTest.h"
#include "fakeit.hpp"

#include "IGraphic.hpp"
#include "SquareTool.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace fakeit;

namespace fakeit_with_mstest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Mock<IGraphics> graphicsMock;
			Fake(Method(graphicsMock, DrawLine));

			SquareTool squareTool;
			squareTool.setStart(0, 0);
			squareTool.setSize(10);
			squareTool.DrawOn(graphicsMock.get());

			Verify(Method(graphicsMock, DrawLine)(0, 0, 10, 0));
			VerifyNoOtherInvocations(Method(graphicsMock, DrawLine));
		}

	};
}
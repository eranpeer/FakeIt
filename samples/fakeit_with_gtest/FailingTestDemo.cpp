#include "gtest/gtest.h"
#include "fakeit.hpp"
#include "IGraphic.hpp"
#include "SquareTool.hpp"

using namespace fakeit;

TEST(TestClass,TestName)
{
	Mock<IGraphics> graphicsMock;
	Fake(Method(graphicsMock, DrawLine));

	SquareTool squareTool;
	squareTool.setStart(0, 0);
	squareTool.setSize(10);
	squareTool.DrawOn(graphicsMock.get());

	Verify(Method(graphicsMock, DrawLine)(0, 0, 10, 0));

	// Will file since there are more invocations.
	VerifyNoOtherInvocations(Method(graphicsMock, DrawLine));
}
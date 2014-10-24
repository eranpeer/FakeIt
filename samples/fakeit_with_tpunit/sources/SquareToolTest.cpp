/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */


#include <string>
#include <iosfwd>
#include <stdexcept>
#include <tuple>

#include "tpunit++.hpp"
#include "fakeit.hpp"

#include "IGraphic.hpp"
#include "SquareTool.hpp"

using namespace fakeit;

struct CustomTestingFramework: tpunit::TestFixture {

	CustomTestingFramework()
			: tpunit::TestFixture(
			//
					TEST(CustomTestingFramework::failingTest)) //
	{
	}

	void failingTest() {
		Mock<IGraphics> graphicsMock;
		Fake(Method(graphicsMock, DrawLine));

		SquareTool squareTool;
		squareTool.setStart(0, 0);
		squareTool.setSize(10);
		squareTool.DrawOn(graphicsMock.get());

		Verify(Method(graphicsMock, DrawLine)(0,0,10,0));
		VerifyNoOtherInvocations(Method(graphicsMock, DrawLine));
	}

} __CustomTestingFramework;

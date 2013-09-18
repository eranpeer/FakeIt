#include "stdafx.h"
#include "CppUnitTest.h"
#include "Mock.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace mock4cpp_tests
{

	TEST_CLASS(DataMembersTest)
	{
	public:

		struct AbstractClass
		{
			AbstractClass() : c(1), s(1), i(1), f(1), d(1), l(1){}

			char c;
			short s;
			int i;
			float f;
			double d;
			long l;

			std::string str;
			int iArr[3];

			virtual void proc() = 0;
		};

		TEST_METHOD(DataMembersAreInitiatedToZero){
			Mock<AbstractClass> mock;
			AbstractClass &i = mock.get();
			Assert::AreEqual((char) 0, i.c);
			Assert::AreEqual((short) 0, i.s);
			Assert::AreEqual((int) 0, i.i);
			Assert::AreEqual((float) 0, i.f);
			Assert::AreEqual((double) 0, i.d);
			Assert::AreEqual((long) 0, i.l);
		}

		TEST_METHOD(SetGetPrimitiveDataMembers){
			Mock<AbstractClass> mock;
			AbstractClass &i = mock.get();
			i.c = 1;
			i.s = 1;
			i.i = 1;
			i.f = 1;
			i.d = 1;
			i.l = 1;
			Assert::AreEqual((char) 1, i.c);
			Assert::AreEqual((short) 1, i.s);
			Assert::AreEqual((int) 1, i.i);
			Assert::AreEqual((float) 1.0, i.f);
			Assert::AreEqual((double) 1.0, i.d);
			Assert::AreEqual((long) 1, i.l);
		}

		TEST_METHOD(StubPrimitiveDataMembers){
			Mock<AbstractClass> mock;
			AbstractClass &i = mock.get();
			mock.Stub(&AbstractClass::c, '1');
			mock.Stub(&AbstractClass::s, (short) 2);
			mock.Stub(&AbstractClass::i, 3);
			mock.Stub(&AbstractClass::f, (float) 4);
			mock.Stub(&AbstractClass::d, (double) 5);
			mock.Stub(&AbstractClass::l, 6);
			Assert::AreEqual('1', i.c);
			Assert::AreEqual((short) 2, i.s);
			Assert::AreEqual((int) 3, i.i);
			Assert::AreEqual((float) 4.0, i.f);
			Assert::AreEqual((double) 5.0, i.d);
			Assert::AreEqual((long) 6, i.l);
		}

		TEST_METHOD(StubObjectDataMembers){
			Mock<AbstractClass> mock;
			AbstractClass &i = mock.get();

			mock.Stub(&AbstractClass::str);
			Assert::AreEqual(std::string(), i.str);

			mock.Stub(&AbstractClass::str, "text");
			Assert::AreEqual(std::string("text"), i.str);

			mock.Stub(&AbstractClass::str, 'a', 'b');
			Assert::AreEqual(std::string("ab"), i.str);
		}

		TEST_METHOD(StubArrayDataMembers){
			Mock<AbstractClass> mock;
			AbstractClass &i = mock.get();

			mock.Stub(&AbstractClass::iArr);
			Assert::AreEqual(0, i.iArr[0]);
			Assert::AreEqual(0, i.iArr[1]);
			Assert::AreEqual(0, i.iArr[2]);

			mock.Stub(&AbstractClass::iArr, 1);
			Assert::AreEqual(1, i.iArr[0]);
			Assert::AreEqual(0, i.iArr[1]);
			Assert::AreEqual(0, i.iArr[2]);

			mock.Stub(&AbstractClass::iArr, 1, 2);
			Assert::AreEqual(1, i.iArr[0]);
			Assert::AreEqual(2, i.iArr[1]);
			Assert::AreEqual(0, i.iArr[2]);

			mock.Stub(&AbstractClass::iArr, 1, 2, 3);
			Assert::AreEqual(1, i.iArr[0]);
			Assert::AreEqual(2, i.iArr[1]);
			Assert::AreEqual(3, i.iArr[2]);
		}
	};
}
/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include "tpunit++.hpp"
#include "fakeit.hpp"

using namespace fakeit;

struct SpyingTests: tpunit::TestFixture {

	SpyingTests() :
			tpunit::TestFixture( //
					TEST(SpyingTests::useOriginalClassMethodIfNotFaked), //
					TEST(SpyingTests::useOriginalClassMethodIfStubbed), //
					TEST(SpyingTests::returnToOrignalClassMethodsOnReset), //
					TEST(SpyingTests::dataMembersAreNotChangedOnReset), //
					TEST(SpyingTests::verifySpyiedMethodWasCalled), //
					TEST(SpyingTests::mockDestructorDoesNotDeleteObject), //
					TEST(SpyingTests::canVerifyMethodAfterSpying),//
					TEST(SpyingTests::canVerifyProcedureAfterSpying),
                    TEST(SpyingTests::restoreObjectOnMockDelete),
					TEST(SpyingTests::spyMultipleMethods),
					TEST(SpyingTests::callMemberMethodFromSpiedMethod),
					TEST(SpyingTests::spyThenVerifyValueArg),
					TEST(SpyingTests::spyMoveOnlyPassedByRef),
					TEST(SpyingTests::spyMoveOnlyWithoutVerify)
					//
	) //
	{
	}

	struct MoveOnlyType {
		const int i_;
		MoveOnlyType(int i) : i_{i} {}
		MoveOnlyType(const MoveOnlyType&) = delete;
		MoveOnlyType(MoveOnlyType&& o) : i_{o.i_} {};
		bool operator==(const MoveOnlyType& o) const { return i_ == o.i_; }
	};

	class SomeClass {
	public:
		int a;
		virtual int func1(int arg) {
			return arg;
		}
		virtual int func2(int arg) {
			return arg;
		}
		virtual void proc(){
		}
		virtual std::string funcTakeByValue(std::string arg) {
			return arg;
		}
		virtual int funcMoveOnlyByRef(const MoveOnlyType& arg) {
			return arg.i_;
		}
		virtual int funcMoveOnlyByValue(MoveOnlyType arg) {
			return arg.i_;
		}
	};

	void useOriginalClassMethodIfNotFaked() {
		SomeClass obj;
		Mock<SomeClass> spy(obj);
		Fake(Method(spy,func1)); // Override to return a default value.
		SomeClass& i = spy.get();
		ASSERT_EQUAL(0, i.func1(1)); // should return default int value (0)
		ASSERT_EQUAL(1, i.func2(1)); // should use original method
	}

	void useOriginalClassMethodIfStubbed() {
		SomeClass obj;
		Mock<SomeClass> spy(obj);
		When(Method(spy,func1)).AlwaysReturn(10); // Override to return 10
		SomeClass& i = spy.get();
		ASSERT_EQUAL(10, i.func1(1)); // should return 10
		ASSERT_EQUAL(1, i.func2(1)); // func2 is not stubbed. should use original method
	}

	void returnToOrignalClassMethodsOnReset() {
		SomeClass obj;
		Mock<SomeClass> spy(obj);
		When(Method(spy,func1)).AlwaysReturn(10); // Override to return 10
		When(Method(spy,func2)).AlwaysReturn(10); // Override to return 10
		spy.Reset();
		SomeClass& i = spy.get();
		ASSERT_EQUAL(1, i.func1(1));  // should use original method
		ASSERT_EQUAL(1, i.func2(1)); // should use original method
	}

	void dataMembersAreNotChangedOnReset() {
		SomeClass obj;
		Mock<SomeClass> spy(obj);

		obj.a = 10;
		spy.Reset();
		SomeClass& i = spy.get();

		ASSERT_EQUAL(10, obj.a);
		ASSERT_EQUAL(10, i.a);
	}

	void verifySpyiedMethodWasCalled() {
		SomeClass obj;
		Mock<SomeClass> spy(obj);
		Fake(Method(spy,func1),Method(spy,func2)); // Override to return a default value.
		SomeClass& i = spy.get();
		i.func1(1);
		Verify(Method(spy,func1));
		ASSERT_THROW(Verify(Method(spy,func2)),fakeit::VerificationException);
	}

	void mockDestructorDoesNotDeleteObject() {
		struct SomeClass {
			bool deleted;
			SomeClass() :
					deleted(false) {
			}
			~SomeClass() {
				deleted = true;
			}
			virtual int func(int arg) {
				return arg;
			}
		};

		SomeClass obj;
		{
			Mock<SomeClass> spy(obj);
		}

		ASSERT_FALSE(obj.deleted);
	}

	void canVerifyMethodAfterSpying() {
		SomeClass obj;
		Mock<SomeClass> mock(obj);
		Spy(Method(mock,func1));
		SomeClass& i = mock.get();
		ASSERT_EQUAL(1, i.func1(1));
		Verify(Method(mock,func1));
		Verify(Method(mock,func1)).Exactly(Once);
		VerifyNoOtherInvocations(mock);
	}

	void canVerifyProcedureAfterSpying() {
		SomeClass obj;
		Mock<SomeClass> mock(obj);
		Spy(Method(mock,proc));
		SomeClass& i = mock.get();
		i.proc();
		Verify(Method(mock,proc));
		Verify(Method(mock,proc)).Exactly(Once);
		VerifyNoOtherInvocations(mock);
	}

	void spyMultipleMethods() {
		SomeClass obj;
		Mock<SomeClass> mock(obj);
		Spy(Method(mock,func1),Method(mock,proc));
		SomeClass& i = mock.get();
		i.func1(1);
		i.proc();
		Verify(Method(mock,proc)).Exactly(Once);
		Verify(Method(mock,func1)).Exactly(Once);
		VerifyNoOtherInvocations(mock);
	}

    void restoreObjectOnMockDelete() {
        SomeClass obj;
        SomeClass& r = obj;
		spy(obj);
        ASSERT_EQUAL(1, r.func1(1));
    }

    void spy(SomeClass& obj) {
		Mock<SomeClass> mock(obj);
		Fake(Method(mock, func1));
		SomeClass &i = mock.get();
		ASSERT_EQUAL(0, i.func1(1));
		ASSERT_EQUAL(0, obj.func1(1));
	}

    class Dummy {
    public:
        virtual void method() {
            printf("Method called\n");
        }

        virtual void callMethod() {
            printf("this: %p\n", (void*)this);
            this->method();
        }
    };

    void callMemberMethodFromSpiedMethod() {
        Dummy instance;
        auto spy = Mock<Dummy>(instance);
        Spy(Method(spy, method));
		Spy(Method(spy, callMethod));
		spy.get().callMethod();
		instance.callMethod();
		Verify(Method(spy, method));
    }

	void spyThenVerifyValueArg()
	{
		SomeClass obj;
		Mock<SomeClass> mock(obj);
		Spy(Method(mock,funcTakeByValue));

		SomeClass &i = mock.get();
		ASSERT_EQUAL("str_arg", i.funcTakeByValue("str_arg"));

		Verify(Method(mock,funcTakeByValue).Using("str_arg")).Once();
	}

	void spyMoveOnlyPassedByRef()
	{
		SomeClass obj;
		Mock<SomeClass> mock(obj);
		Spy(Method(mock,funcMoveOnlyByRef));

		SomeClass &i = mock.get();
		MoveOnlyType mot{5};
		ASSERT_EQUAL(5, i.funcMoveOnlyByRef(mot));

		Verify(Method(mock,funcMoveOnlyByRef).Using(MoveOnlyType{5})).Once();
	}

	void spyMoveOnlyWithoutVerify()
	{
		SomeClass obj;
		Mock<SomeClass> mock(obj);
		SpyWithoutVerify(Method(mock,funcMoveOnlyByValue));

		SomeClass &i = mock.get();
		ASSERT_EQUAL(10, i.funcMoveOnlyByValue(MoveOnlyType{10}));
	}

} __SpyingTests;

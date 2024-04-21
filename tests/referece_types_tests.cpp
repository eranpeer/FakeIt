/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#include <string>
#include <queue>

#include "tpunit++.hpp"
#include "fakeit.hpp"

using namespace fakeit;

struct ReferenceTypesTests: tpunit::TestFixture {

	class AbstractType {
	public:
		virtual ~AbstractType() = default;

		virtual const std::string& getContent() = 0;

		// Return true if object was sliced, false otherwise.
		virtual bool wasObjectSliced() {
			return true;
		}
	};

	class ConcreteType: public AbstractType {
	public:
		std::string content;
		ConcreteType(const std::string& newContent = "defaultContent") :
				content{newContent} {
		}
		const std::string& getContent() override {
			return content;
		}
		bool wasObjectSliced() override {
			return false;
		}

		bool operator==(const ConcreteType& other) const {
			return (other.content == this->content);
		}

	};

	struct ReferenceInterface {
		virtual int& returnIntByRef() = 0;
		virtual AbstractType& returnAbstractTypeByRef() = 0;
		virtual ConcreteType& returnConcreteTypeByRef() = 0;
		virtual const std::string& returnStringByConstRef() = 0;
		virtual std::string& returnStringByRef() = 0;
		virtual const std::unique_ptr<std::string>& returnMoveOnlyByConstRef() = 0;
		virtual std::unique_ptr<std::string>& returnMoveOnlyByRef() = 0;
		virtual std::string&& returnStringByRValRef() = 0;
		virtual std::unique_ptr<std::string>&& returnMoveOnlyByRValRef() = 0;
	};

	ReferenceTypesTests() :
			tpunit::TestFixture(
					//
					TEST(ReferenceTypesTests::implicitStubbingDefaultReturnValues),
					TEST(ReferenceTypesTests::explicitStubbingDefaultReturnValues),
					TEST(ReferenceTypesTests::explicitStubbingReturnValues),
					TEST(ReferenceTypesTests::explicitStubbingDefaultReturnValues_with_AlwaysReturn),
					TEST(ReferenceTypesTests::explicitStubbingReturnValues_with_AlwaysReturn),
					TEST(ReferenceTypesTests::explicitStubbingReturnValues_by_assignment),
					TEST(ReferenceTypesTests::explicitStubbingReturnCaptureByCopy),
					TEST(ReferenceTypesTests::explicitStubbingReturnCaptureByMove),
					TEST(ReferenceTypesTests::explicitStubbingReturnCaptureByTemporary),
					TEST(ReferenceTypesTests::explicitStubbingReturnCaptureByCopy_with_AlwaysReturn),
					TEST(ReferenceTypesTests::explicitStubbingReturnCaptureByMove_with_AlwaysReturn),
					TEST(ReferenceTypesTests::explicitStubbingReturnCaptureByTemporary_with_AlwaysReturn)
					//
							) {
	}

	void implicitStubbingDefaultReturnValues() {
		Mock<ReferenceInterface> mock;
#ifndef FAKEIT_DISABLE_UBSAN_TRIGGERING_TESTS
		Fake( //
				Method(mock,returnIntByRef), //
				Method(mock,returnAbstractTypeByRef), //
				Method(mock,returnConcreteTypeByRef) //
				);
#else
		Fake( //
				Method(mock,returnIntByRef), //
				Method(mock,returnConcreteTypeByRef) //
				);
#endif

		ReferenceInterface & i = mock.get();

		// Fundamental types are initiated to 0.
		// Return a reference to the default value.
		ASSERT_EQUAL(0, i.returnIntByRef());

		// Concrete types are initiated by default ctor.
		// Return a reference to the default value.
		ASSERT_EQUAL(ConcreteType(), i.returnConcreteTypeByRef());

#ifndef FAKEIT_DISABLE_UBSAN_TRIGGERING_TESTS
		// For abstract types return a reference to nullptr.
		ASSERT_EQUAL(nullptr, &i.returnAbstractTypeByRef());
#endif
	}

	void explicitStubbingDefaultReturnValues() {
		Mock<ReferenceInterface> mock;		//
		When(Method(mock,returnIntByRef)).Return(); //
#ifndef FAKEIT_DISABLE_UBSAN_TRIGGERING_TESTS
		When(Method(mock,returnAbstractTypeByRef)).Return(); //
#endif
		When(Method(mock,returnConcreteTypeByRef)).Return(); //

		ReferenceInterface & i = mock.get();

		// Fundamental types are initiated to 0.
		// Return a reference to the default value.
		ASSERT_EQUAL(0, i.returnIntByRef());

		// Concrete types are initiated by default ctor.
		// Return a reference to the default value.
		ASSERT_EQUAL(ConcreteType(), i.returnConcreteTypeByRef());

#ifndef FAKEIT_DISABLE_UBSAN_TRIGGERING_TESTS
		// For abstract types return a reference to nullptr.
		ASSERT_EQUAL(nullptr, &i.returnAbstractTypeByRef());
#endif
	}

	void explicitStubbingReturnValues() {
		Mock<ReferenceInterface> mock;		//

		int num { 1 };
		ConcreteType c;
		AbstractType& a = c;
		When(Method(mock,returnIntByRef)).Return(num); //
		When(Method(mock,returnConcreteTypeByRef)).Return(c); //
		When(Method(mock,returnAbstractTypeByRef)).Return(a); //

		ReferenceInterface & i = mock.get();

		// Fundamental types are initiated to 0.
		// Return a reference to the default value.
		ASSERT_EQUAL(1, i.returnIntByRef());

		// Concrete types are initiated by default ctor.
		// Return a reference to the default value.
		ASSERT_EQUAL(&c, &i.returnConcreteTypeByRef());

		// For abstract types return a reference to nullptr.
		ASSERT_EQUAL(&c, &i.returnAbstractTypeByRef());
	}

	void explicitStubbingReturnValues_with_AlwaysReturn() {
		Mock<ReferenceInterface> mock;		//

		int num { 1 };
		ConcreteType c;
		AbstractType& a = c;
		When(Method(mock,returnIntByRef)).AlwaysReturn(num); //
		When(Method(mock,returnConcreteTypeByRef)).AlwaysReturn(c); //
		When(Method(mock,returnAbstractTypeByRef)).AlwaysReturn(a); //

		ReferenceInterface & i = mock.get();

		// Fundamental types are initiated to 0.
		// Return a reference to the default value.
		ASSERT_EQUAL(1, i.returnIntByRef());

		// Concrete types are initiated by default ctor.
		// Return a reference to the default value.
		ASSERT_EQUAL(&c, &i.returnConcreteTypeByRef());

		// For abstract types return a reference to nullptr.
		ASSERT_EQUAL(&c, &i.returnAbstractTypeByRef());
	}

	void explicitStubbingReturnValues_by_assignment() {
		Mock<ReferenceInterface> mock;		//

		int num { 1 };
		ConcreteType c;
		AbstractType& a = c;
		Method(mock,returnIntByRef) = num;
		Method(mock,returnConcreteTypeByRef) = c;
		Method(mock,returnAbstractTypeByRef) = a;

		ReferenceInterface & i = mock.get();

		// Fundamental types are initiated to 0.
		// Return a reference to the default value.
		ASSERT_EQUAL(1, i.returnIntByRef());

		// Concrete types are initiated by default ctor.
		// Return a reference to the default value.
		ASSERT_EQUAL(&c, &i.returnConcreteTypeByRef());

		// For abstract types return a reference to nullptr.
		ASSERT_EQUAL(&c, &i.returnAbstractTypeByRef());
	}

	void explicitStubbingDefaultReturnValues_with_AlwaysReturn() {
		Mock<ReferenceInterface> mock;
		When(Method(mock,returnIntByRef)).AlwaysReturn();
#ifndef FAKEIT_DISABLE_UBSAN_TRIGGERING_TESTS
		When(Method(mock,returnAbstractTypeByRef)).AlwaysReturn(); //
#endif
		When(Method(mock,returnConcreteTypeByRef)).AlwaysReturn(); //

		ReferenceInterface & i = mock.get();

		// Fundamental types are initiated to 0.
		// Return a reference to the default value.
		ASSERT_EQUAL(0, i.returnIntByRef());

		// Concrete types are initiated by default ctor.
		// Return a reference to the default value.
		ASSERT_EQUAL(ConcreteType(), i.returnConcreteTypeByRef());

#ifndef FAKEIT_DISABLE_UBSAN_TRIGGERING_TESTS
		// For abstract types return a reference to nullptr.
		ASSERT_EQUAL(nullptr, &i.returnAbstractTypeByRef());
#endif
	}

	void explicitStubbingReturnCaptureByCopy() {
		Mock<ReferenceInterface> mock;

		// add scope so we know we are copying
		{
			std::string aString{"aString"};
			std::string bString{"bString"};
			int num = 1;
			ConcreteType concrete{"myConcreteType"};

			// explicit copy here
			When(Method(mock, returnStringByConstRef)).ReturnCapture(aString);
			When(Method(mock, returnStringByRValRef)).ReturnCapture(bString);
			When(Method(mock, returnIntByRef)).ReturnCapture(num);
			When(Method(mock, returnAbstractTypeByRef)).ReturnCapture(concrete);

			// modify now so know whether or not is was copied
			aString = "modified";
			bString = "modified";
			num = 2;
			concrete.content = "modified";
		}

		ReferenceInterface& i = mock.get();

		EXPECT_EQUAL("aString", i.returnStringByConstRef());
		EXPECT_EQUAL("bString", i.returnStringByRValRef());
		EXPECT_EQUAL(1, i.returnIntByRef());
		AbstractType& abstract = i.returnAbstractTypeByRef();
		EXPECT_FALSE(abstract.wasObjectSliced());
		EXPECT_EQUAL("myConcreteType", abstract.getContent());
	}

	void explicitStubbingReturnCaptureByMove() {
		Mock<ReferenceInterface> mock;

		// add scope so we know we are moving
		{
			std::string aString{"aString"};
			std::string bString{"bString"};
			std::string cString{"cString"};
			std::unique_ptr<std::string> aPtrString{new std::string{"aPtrString"}};
			std::unique_ptr<std::string> bPtrString{new std::string{"bPtrString"}};
			std::unique_ptr<std::string> cPtrString{new std::string{"cPtrString"}};
			ConcreteType concrete{"myConcreteType"};

			// explicit move here
			When(Method(mock, returnStringByConstRef)).ReturnCapture(std::move(aString));
			When(Method(mock, returnStringByRef)).ReturnCapture(std::move(bString));
			When(Method(mock, returnStringByRValRef)).ReturnCapture(std::move(cString));
			When(Method(mock, returnMoveOnlyByConstRef)).ReturnCapture(std::move(aPtrString));
			When(Method(mock, returnMoveOnlyByRef)).ReturnCapture(std::move(bPtrString));
			When(Method(mock, returnMoveOnlyByRValRef)).ReturnCapture(std::move(cPtrString));
			When(Method(mock, returnAbstractTypeByRef)).ReturnCapture(std::move(concrete));

			// Verify objects were moved.
			EXPECT_TRUE(aString.empty());
			EXPECT_TRUE(bString.empty());
			EXPECT_TRUE(cString.empty());
			EXPECT_EQUAL(aPtrString, nullptr);
			EXPECT_EQUAL(bPtrString, nullptr);
			EXPECT_EQUAL(cPtrString, nullptr);
			EXPECT_TRUE(concrete.content.empty());
		}

		ReferenceInterface& i = mock.get();

		EXPECT_EQUAL("aString", i.returnStringByConstRef());
		EXPECT_EQUAL("bString", i.returnStringByRef());
		EXPECT_EQUAL("cString", i.returnStringByRValRef());
		EXPECT_EQUAL("aPtrString", *i.returnMoveOnlyByConstRef());
		EXPECT_EQUAL("bPtrString", *i.returnMoveOnlyByRef());
		EXPECT_EQUAL("cPtrString", *i.returnMoveOnlyByRValRef());
		AbstractType& abstract = i.returnAbstractTypeByRef();
		EXPECT_FALSE(abstract.wasObjectSliced());
		EXPECT_EQUAL("myConcreteType", abstract.getContent());
	}

	void explicitStubbingReturnCaptureByTemporary() {
		Mock<ReferenceInterface> mock;

		{
			When(Method(mock, returnStringByConstRef)).ReturnCapture(std::string{"aString"});
			When(Method(mock, returnStringByRef)).ReturnCapture(std::string{"bString"});
			When(Method(mock, returnStringByRValRef)).ReturnCapture(std::string{"cString"});
			When(Method(mock, returnMoveOnlyByConstRef)).ReturnCapture(std::unique_ptr<std::string>(new std::string{"aPtrString"}));
			When(Method(mock, returnMoveOnlyByRef)).ReturnCapture(std::unique_ptr<std::string>(new std::string{"bPtrString"}));
			When(Method(mock, returnMoveOnlyByRValRef)).ReturnCapture(std::unique_ptr<std::string>(new std::string{"cPtrString"}));
			When(Method(mock, returnAbstractTypeByRef)).ReturnCapture(ConcreteType{"myConcreteType"});
		}

		ReferenceInterface& i = mock.get();

		EXPECT_EQUAL("aString", i.returnStringByConstRef());
		EXPECT_EQUAL("bString", i.returnStringByRef());
		EXPECT_EQUAL("cString", i.returnStringByRValRef());
		EXPECT_EQUAL("aPtrString", *i.returnMoveOnlyByConstRef());
		EXPECT_EQUAL("bPtrString", *i.returnMoveOnlyByRef());
		EXPECT_EQUAL("cPtrString", *i.returnMoveOnlyByRValRef());
		AbstractType& abstract = i.returnAbstractTypeByRef();
		EXPECT_FALSE(abstract.wasObjectSliced());
		EXPECT_EQUAL("myConcreteType", abstract.getContent());
	}

	void explicitStubbingReturnCaptureByCopy_with_AlwaysReturn() {
		Mock<ReferenceInterface> mock;

		// add scope so we know we are copying
		{
			std::string aString{"aString"};
			std::string bString{"bString"};
			int num = 1;
			ConcreteType concrete{"myConcreteType"};

			// explicit copy here
			When(Method(mock, returnStringByConstRef)).AlwaysReturnCapture(aString);
			When(Method(mock, returnStringByRValRef)).AlwaysReturnCapture(bString);
			When(Method(mock, returnIntByRef)).AlwaysReturnCapture(num);
			When(Method(mock, returnAbstractTypeByRef)).AlwaysReturnCapture(concrete);

			// modify now so know whether or not is was copied
			aString = "modified";
			bString = "modified";
			num = 2;
			concrete.content = "modified";
		}

		ReferenceInterface& i = mock.get();

		EXPECT_EQUAL("aString", i.returnStringByConstRef());
		EXPECT_EQUAL("aString", i.returnStringByConstRef());
		EXPECT_EQUAL("bString", i.returnStringByRValRef());
		EXPECT_EQUAL("bString", i.returnStringByRValRef());
		{
			std::string bStr = i.returnStringByRValRef();
			EXPECT_EQUAL("bString", bStr);
			EXPECT_TRUE(i.returnStringByRValRef().empty());
		}
		EXPECT_EQUAL(1, i.returnIntByRef());
		EXPECT_EQUAL(1, i.returnIntByRef());
		{
			AbstractType& abstract = i.returnAbstractTypeByRef();
			EXPECT_FALSE(abstract.wasObjectSliced());
			EXPECT_EQUAL("myConcreteType", abstract.getContent());
		}
		{
			AbstractType& abstract = i.returnAbstractTypeByRef();
			EXPECT_FALSE(abstract.wasObjectSliced());
			EXPECT_EQUAL("myConcreteType", abstract.getContent());
		}
	}

	void explicitStubbingReturnCaptureByMove_with_AlwaysReturn() {
		Mock<ReferenceInterface> mock;

		// add scope so we know we are moving
		{
			std::string aString{"aString"};
			std::string bString{"bString"};
			std::string cString{"cString"};
			std::unique_ptr<std::string> aPtrString{new std::string{"aPtrString"}};
			std::unique_ptr<std::string> bPtrString{new std::string{"bPtrString"}};
			std::unique_ptr<std::string> cPtrString{new std::string{"cPtrString"}};
			ConcreteType concrete{"myConcreteType"};

			// explicit move here
			When(Method(mock, returnStringByConstRef)).AlwaysReturnCapture(std::move(aString));
			When(Method(mock, returnStringByRef)).AlwaysReturnCapture(std::move(bString));
			When(Method(mock, returnStringByRValRef)).AlwaysReturnCapture(std::move(cString));
			When(Method(mock, returnMoveOnlyByConstRef)).AlwaysReturnCapture(std::move(aPtrString));
			When(Method(mock, returnMoveOnlyByRef)).AlwaysReturnCapture(std::move(bPtrString));
			When(Method(mock, returnMoveOnlyByRValRef)).AlwaysReturnCapture(std::move(cPtrString));
			When(Method(mock, returnAbstractTypeByRef)).AlwaysReturnCapture(std::move(concrete));

			// Verify objects were moved.
			EXPECT_TRUE(aString.empty());
			EXPECT_TRUE(bString.empty());
			EXPECT_TRUE(cString.empty());
			EXPECT_EQUAL(aPtrString, nullptr);
			EXPECT_EQUAL(bPtrString, nullptr);
			EXPECT_EQUAL(cPtrString, nullptr);
			EXPECT_TRUE(concrete.content.empty());
		}

		ReferenceInterface& i = mock.get();

		EXPECT_EQUAL("aString", i.returnStringByConstRef());
		EXPECT_EQUAL("aString", i.returnStringByConstRef());
		EXPECT_EQUAL("bString", i.returnStringByRef());
		EXPECT_EQUAL("bString", i.returnStringByRef());
		EXPECT_EQUAL("cString", i.returnStringByRValRef());
		EXPECT_EQUAL("cString", i.returnStringByRValRef());
		{
			std::string cStr = i.returnStringByRValRef();
			EXPECT_EQUAL("cString", cStr);
			EXPECT_TRUE(i.returnStringByRValRef().empty());
		}
		EXPECT_EQUAL("aPtrString", *i.returnMoveOnlyByConstRef());
		EXPECT_EQUAL("aPtrString", *i.returnMoveOnlyByConstRef());
		EXPECT_EQUAL("bPtrString", *i.returnMoveOnlyByRef());
		EXPECT_EQUAL("bPtrString", *i.returnMoveOnlyByRef());
		EXPECT_EQUAL("cPtrString", *i.returnMoveOnlyByRValRef());
		EXPECT_EQUAL("cPtrString", *i.returnMoveOnlyByRValRef());
		{
			std::unique_ptr<std::string> cPtrStr = i.returnMoveOnlyByRValRef();
			EXPECT_EQUAL("cPtrString", *cPtrStr);
			EXPECT_EQUAL(i.returnMoveOnlyByRValRef(), nullptr);
		}
		{
			AbstractType& abstract = i.returnAbstractTypeByRef();
			EXPECT_FALSE(abstract.wasObjectSliced());
			EXPECT_EQUAL("myConcreteType", abstract.getContent());
		}
		{
			AbstractType& abstract = i.returnAbstractTypeByRef();
			EXPECT_FALSE(abstract.wasObjectSliced());
			EXPECT_EQUAL("myConcreteType", abstract.getContent());
		}
	}

	void explicitStubbingReturnCaptureByTemporary_with_AlwaysReturn() {
		Mock<ReferenceInterface> mock;

		{
			When(Method(mock, returnStringByConstRef)).AlwaysReturnCapture(std::string{"aString"});
			When(Method(mock, returnStringByRef)).AlwaysReturnCapture(std::string{"bString"});
			When(Method(mock, returnStringByRValRef)).AlwaysReturnCapture(std::string{"cString"});
			When(Method(mock, returnMoveOnlyByConstRef)).AlwaysReturnCapture(std::unique_ptr<std::string>(new std::string{"aPtrString"}));
			When(Method(mock, returnMoveOnlyByRef)).AlwaysReturnCapture(std::unique_ptr<std::string>(new std::string{"bPtrString"}));
			When(Method(mock, returnMoveOnlyByRValRef)).AlwaysReturnCapture(std::unique_ptr<std::string>(new std::string{"cPtrString"}));
			When(Method(mock, returnAbstractTypeByRef)).AlwaysReturnCapture(ConcreteType{"myConcreteType"});
		}

		ReferenceInterface& i = mock.get();

		EXPECT_EQUAL("aString", i.returnStringByConstRef());
		EXPECT_EQUAL("aString", i.returnStringByConstRef());
		EXPECT_EQUAL("bString", i.returnStringByRef());
		EXPECT_EQUAL("bString", i.returnStringByRef());
		EXPECT_EQUAL("cString", i.returnStringByRValRef());
		EXPECT_EQUAL("cString", i.returnStringByRValRef());
		{
			std::string cStr = i.returnStringByRValRef();
			EXPECT_EQUAL("cString", cStr);
			EXPECT_TRUE(i.returnStringByRValRef().empty());
		}
		EXPECT_EQUAL("aPtrString", *i.returnMoveOnlyByConstRef());
		EXPECT_EQUAL("aPtrString", *i.returnMoveOnlyByConstRef());
		EXPECT_EQUAL("bPtrString", *i.returnMoveOnlyByRef());
		EXPECT_EQUAL("bPtrString", *i.returnMoveOnlyByRef());
		EXPECT_EQUAL("cPtrString", *i.returnMoveOnlyByRValRef());
		EXPECT_EQUAL("cPtrString", *i.returnMoveOnlyByRValRef());
		{
			std::unique_ptr<std::string> cPtrStr = i.returnMoveOnlyByRValRef();
			EXPECT_EQUAL("cPtrString", *cPtrStr);
			EXPECT_EQUAL(i.returnMoveOnlyByRValRef(), nullptr);
		}
		{
			AbstractType& abstract = i.returnAbstractTypeByRef();
			EXPECT_FALSE(abstract.wasObjectSliced());
			EXPECT_EQUAL("myConcreteType", abstract.getContent());
		}
		{
			AbstractType& abstract = i.returnAbstractTypeByRef();
			EXPECT_FALSE(abstract.wasObjectSliced());
			EXPECT_EQUAL("myConcreteType", abstract.getContent());
		}
	}

} __ReferenceTypesTests;

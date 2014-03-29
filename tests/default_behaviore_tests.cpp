#if defined (__GNUG__)

#include <string>
#include <queue>

#include "tpunit++.hpp"
#include "fakeit.h"
#include "fakeit/FakeitExceptions.h"

using namespace fakeit;

struct DefaultBehavioreTests: tpunit::TestFixture {
	DefaultBehavioreTests() :
			tpunit::TestFixture(
			//
					TEST(DefaultBehavioreTests::scalar_types_should_return_zero)
					//
							) {
	}

	enum Color {
		RED = 1, GREEN = 2, BLUE = 3
	};

	struct ScalarFuctions {
		virtual bool boolFunc() = 0;
		virtual char charFunc() = 0;
		virtual char16_t char16Func() = 0;
		virtual char32_t char32Func() = 0;
		virtual wchar_t wcharFunc() = 0;
		virtual short shortFunc() = 0;
		virtual int intFunc() = 0;
		virtual long longFunc() = 0;
		virtual long long longLongFunc() = 0;
		virtual float floatFunc() = 0;
		virtual double doubleFunc() = 0;
		virtual long double longDoubleFunc() = 0;

		virtual Color enumFunc() = 0;

		virtual int * pIntFunc() = 0;
		virtual ScalarFuctions * pScalarFuctionsfunc() = 0;
		virtual std::nullptr_t nullptrFunc() = 0;

		typedef bool (ScalarFuctions::*func)();
		virtual func pMemberFunc() = 0;
	};

	void scalar_types_should_return_zero() {
		Mock<ScalarFuctions> mock;
		Fake(mock[&ScalarFuctions::boolFunc]);
		Fake(mock[&ScalarFuctions::charFunc]);
		Fake(mock[&ScalarFuctions::char16Func]);
		Fake(mock[&ScalarFuctions::char32Func]);
		Fake(mock[&ScalarFuctions::wcharFunc]);
		Fake(mock[&ScalarFuctions::shortFunc]);
		Fake(mock[&ScalarFuctions::intFunc]);
		Fake(mock[&ScalarFuctions::longFunc]);
		Fake(mock[&ScalarFuctions::longLongFunc]);
		Fake(mock[&ScalarFuctions::floatFunc]);
		Fake(mock[&ScalarFuctions::doubleFunc]);
		Fake(mock[&ScalarFuctions::longDoubleFunc]);
		Fake(mock[&ScalarFuctions::enumFunc]);
		Fake(mock[&ScalarFuctions::pIntFunc]);
		Fake(mock[&ScalarFuctions::pScalarFuctionsfunc]);
		Fake(mock[&ScalarFuctions::nullptrFunc]);
		Fake(mock[&ScalarFuctions::pMemberFunc]);

		ScalarFuctions &i = mock.get();

		//Default behavior of a scalar function is to return 0/false/null

		ASSERT_EQUAL(false, i.boolFunc());
		ASSERT_EQUAL((char ) 0, i.charFunc());
		ASSERT_EQUAL((int )(char16_t ) 0, (int )i.char16Func());
		ASSERT_EQUAL((char32_t ) 0, i.char32Func());
		ASSERT_EQUAL((wchar_t ) 0, i.wcharFunc());
		ASSERT_EQUAL((short ) 0, i.shortFunc());
		ASSERT_EQUAL((int ) 0, i.intFunc());
		ASSERT_EQUAL((long ) 0, i.longFunc());
		ASSERT_EQUAL((long )0, (long )i.longLongFunc());
		ASSERT_EQUAL((float ) 0, i.floatFunc());
		ASSERT_EQUAL((double ) 0, i.doubleFunc());
		ASSERT_EQUAL((double ) 0, (double ) i.longDoubleFunc());
		ASSERT_EQUAL(0, (int ) i.enumFunc());
		ASSERT_EQUAL(nullptr, i.pIntFunc());
		ASSERT_EQUAL(nullptr, i.pScalarFuctionsfunc());
		ASSERT_EQUAL(nullptr, i.nullptrFunc());
		ASSERT_EQUAL(0, union_cast<int>(i.pMemberFunc()));
	}

} __DefaultBehaviore;

#endif

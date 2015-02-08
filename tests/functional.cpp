//
//#include <string>
//#include <queue>
//
//#include "tpunit++.hpp"
//#include "fakeit.hpp"
//#include "fakeit/Functional.hpp"
//
//using namespace fakeit;
//
//struct Functional: tpunit::TestFixture {
//	Functional() :
//	tpunit::TestFixture(
//			TEST( Functional::can_access_polymorphic_members ),
//      TEST( Functional::can_access_polymorphic_constness ),
//      TEST( Functional::can_access_polymorphic_non_const ),
//      TEST( Functional::can_access_polymorphic_const )
//	) {
//	}
//
//	struct SomeInterface {
//		virtual int func(int) = 0;
//		virtual int func(double) = 0;
//		virtual int proc() = 0;
//    virtual int proc() const = 0;
//	};
//
//	void can_access_polymorphic_members() {
//		Mock<SomeInterface> mock;
//		SomeInterface &i = mock.get();
//
//    member_function<SomeInterface, int(int) >    func_int( &SomeInterface::func );
//    member_function<SomeInterface, int(double) > func_double( &SomeInterface::func );
//
//    When( mock[ *func_int    ] ).Return(1);
//    When( mock[ *func_double ] ).Return(2);
//
//		ASSERT_EQUAL(1, i.func(1)    );
//		ASSERT_EQUAL(2, i.func(1.0)  );
//	}
//
//	void can_access_polymorphic_constness() {
//		Mock<SomeInterface> mock;
//		SomeInterface &i = mock.get();
//		const SomeInterface &const_i = mock.get();
//
//    member_function<SomeInterface, int() >       proc( &SomeInterface::proc );
//    const_member_function<SomeInterface, int() >       const_proc( &SomeInterface::proc );
//
//    When( mock[ *proc        ] ).Return(3);
//    When( mock[ *const_proc  ] ).Return(4);
//
//		ASSERT_EQUAL(3, i.proc()     );
//		ASSERT_EQUAL(4, const_i.proc() );
//	}
//
//	void can_access_polymorphic_non_const() {
//		Mock<SomeInterface> mock;
//		SomeInterface &i = mock.get();
//
//    member_function<SomeInterface, int() >       proc( &SomeInterface::proc );
//
//    When( mock[ *proc        ] ).Return(3);
//
//		ASSERT_EQUAL(3, i.proc()     );
//	}
//
//	void can_access_polymorphic_const() {
//		Mock<SomeInterface> mock;
//		const SomeInterface &const_i = mock.get();
//
//    const_member_function<SomeInterface, int() >       const_proc( &SomeInterface::proc );
//
//    When( mock[ *const_proc ] ).Return(4);
//
//		ASSERT_EQUAL(4, const_i.proc() );
//	}
//} __Functional;

// These tests only make sense with the Visual Studio compiler, since only it has the calling convention options.
#ifdef _MSC_VER

#include "tpunit++.hpp"
#include "fakeit.hpp"

#include <type_traits>

using namespace fakeit;

struct Host {
public:
	void foo() const volatile {}
	void bar() {}
};

using fooType = decltype( &Host::foo );
using barType = decltype( &Host::bar );


void ( Host::* ptr )( );

int gg()
{
	return 1;
}

using cleanedFooType = func_traits::remove_cv_t< fooType >;

template<typename A, typename B>
struct assert_same {
	static_assert( std::is_same< A, B>::value, "They were not the same" );
};

void acceptsCorrect( barType param ) {}

void callsWithWhatWeHave()
{
	cleanedFooType cft = nullptr;
	acceptsCorrect( cft );
}

template<typename R>
R foo( R (*func)() )
{
	return (*func)();
}

template<typename Func>
auto bar( Func func ) -> decltype( foo( func ) )
{
	return foo( func );
}

int f()
{
	return 4;
}

void inst()
{
	int x = bar( &f );
}

// Tests for the MSVC calling conventions.
struct MscCallingConventionTests : tpunit::TestFixture {
	MscCallingConventionTests() :
		tpunit::TestFixture(
			TEST( MscCallingConventionTests::cdecl_convention ), //
			TEST( MscCallingConventionTests::stdcall_convention ), //
			TEST( MscCallingConventionTests::thiscall_convention )//
		)  //
	{
	}

	// A class where all the members use the __cdecl convention.
	// This is the default on x64.
	class CdeclClass{
	public:
		virtual void __cdecl DoThing() {}

		virtual int __cdecl GetValue()
		{
			return 4;
		}
	};

	// Mock a class with cdecl members.
	void cdecl_convention()
	{
		// Mock the class.
		Mock< CdeclClass > mockCdecl;
		Fake( Method( mockCdecl, DoThing ) );
		When( Method( mockCdecl, GetValue ) ).AlwaysReturn( 5 );

		// Invoke the mocked methods.
		CdeclClass& instance = mockCdecl.get();
		instance.DoThing();
		ASSERT_EQUAL( 5, instance.GetValue() );
	};

	// A class where all the members use the __stdcall convention.
	// This is how COM objects are.
	class StdcallClass{
	public:
		virtual void __stdcall DoThing() {}

		virtual int __stdcall GetValue()
		{
			return 4;
		}
	};

	// Mock a class with stdcall members.
	void stdcall_convention()
	{
		// Mock the class.
		Mock< StdcallClass > mock;
		Fake( Method( mock, DoThing ) );
		When( Method( mock, GetValue ) ).AlwaysReturn( 5 );

		// Invoke the mocked methods.
		StdcallClass& instance = mock.get();
		instance.DoThing();
		ASSERT_EQUAL( 5, instance.GetValue() );
	};

	// A class where all the members use the __stdcall convention.
	// This is the the default for member functions on 32-bit MSVC.
	class ThiscallClass{
	public:
		virtual void __thiscall DoThing() {}

		virtual int __thiscall GetValue()
		{
			return 4;
		}
	};

	// Mock a class with thiscall members.
	void thiscall_convention()
	{
		// Mock the class.
		Mock< ThiscallClass > mock;
		Fake( Method( mock, DoThing ) );
		When( Method( mock, GetValue ) ).AlwaysReturn( 5 );

		// Invoke the mocked methods.
		ThiscallClass& instance = mock.get();
		instance.DoThing();
		ASSERT_EQUAL( 5, instance.GetValue() );
	};
};

#endif // _MSC_VER

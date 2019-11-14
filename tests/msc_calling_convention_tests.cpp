// These tests only make sense with the Visual Studio compiler, since only it has the calling convention options.
#ifdef _MSC_VER

#include "tpunit++.hpp"
#include "fakeit.hpp"

using namespace fakeit;

// Tests for the MSVC calling conventions.
struct MscCallingConventionTests : tpunit::TestFixture {
	MscCallingConventionTests() :
		tpunit::TestFixture(
			TEST( MscCallingConventionTests::cdecl_convention )//
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
};

#endif // _MSC_VER

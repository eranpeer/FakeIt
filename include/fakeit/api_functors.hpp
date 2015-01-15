#pragma once

namespace fakeit {

	static UsingFunctor Using(Fakeit);
	static VerifyFunctor Verify(Fakeit);
	static VerifyNoOtherInvocationsFunctor VerifyNoOtherInvocations(Fakeit);
	static SpyFunctor Spy;
	static FakeFunctor Fake;
	static WhenFunctor When;

	template<class T>
	class SilenceUnusedVariableWarnings {

		void use(void *) {
		}

		SilenceUnusedVariableWarnings() {
			use(&Fake);
			use(&When);
			use(&Spy);
			use(&Using);
			use(&Verify);
			use(&VerifyNoOtherInvocations);
			use(&_);
		}
	};

}

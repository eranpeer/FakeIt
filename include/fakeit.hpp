#ifndef fakeit_h__
#define fakeit_h__

#include "fakeit/Mock.hpp"
#include "fakeit/WhenFunctor.hpp"
#include "fakeit/FakeFunctor.hpp"
#include "fakeit/UsingFunctor.hpp"
#include "fakeit/VerifyFunctor.hpp"
#include "fakeit/VerifyNoOtherInvocationsFunctor.hpp"
#include "fakeit/SpyFunctor.hpp"

#ifdef _MSC_VER
#define __func__ __FUNCTION__
#endif

#define Method( mock , method ) \
	mock[&std::remove_reference<decltype(mock.get())>::type::method].setMethodDetails(#mock,#method)

#define Verify( ... ) \
		Verify( __VA_ARGS__ ).setFileInfo(__FILE__, __LINE__, __func__)

#define Using( ... ) \
		Using( __VA_ARGS__ )

#define VerifyNoOtherInvocations( ... ) \
	VerifyNoOtherInvocations( __VA_ARGS__ ).setFileInfo(__FILE__, __LINE__, __func__)

#define Fake( ... ) \
	Fake( __VA_ARGS__ )

#define When( call ) \
	When(call)

#endif // fakeit_h__

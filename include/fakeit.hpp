#ifndef fakeit_h__
#define fakeit_h__

#include "fakeit/Mock.hpp"
#include "fakeit/WhenFunctor.hpp"
#include "fakeit/FakeFunctor.hpp"
#include "fakeit/UsingFunctor.hpp"
#include "fakeit/VerifyFunctor.hpp"
#include "fakeit/VerifyNoOtherInvocationsFunctor.hpp"
#include "fakeit/SpyFunctor.hpp"

#ifdef _WIN32
#define __func__ __FUNCTION__
#endif

#define CALL( mock , method ) \
	mock[&std::remove_reference<decltype(mock.get())>::type::method].setMethodDetails(#mock,#method)

#define Verify( ... ) \
        fakeit::Verify( __VA_ARGS__ ).setFileInfo(__FILE__,__LINE__, __func__)

#define Using( ... ) \
        fakeit::Using( __VA_ARGS__ )

#define VerifyNoOtherInvocations( ... ) \
    fakeit::VerifyNoOtherInvocations( __VA_ARGS__ ).setFileInfo(__FILE__,__LINE__, __func__)

#define Fake( ... ) \
    fakeit::Fake( __VA_ARGS__ )

#define When( call ) \
    fakeit::When(call)

#endif // fakeit_h__

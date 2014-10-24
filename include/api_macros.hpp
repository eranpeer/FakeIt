#pragma once


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



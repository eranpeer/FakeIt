#pragma once

#ifdef _MSC_VER
#define __func__ __FUNCTION__
#endif

namespace fakeit {
    template<typename R, typename... Args >
    class Prototype;

    template<typename R, typename... Args >
    struct Prototype < R(Args...) > {

        typedef R Type(Args...);

        template<class C>
        struct MemberType {
            typedef Type(C::*type);
            static type get(type t){
                return t;
            }
        };
    };
}

#define Dtor( mock ) \
	mock.dtor().setMethodDetails(#mock,"destructor")

#define Method( mock , method ) \
	mock[&std::remove_reference<decltype(mock.get())>::type::method].setMethodDetails(#mock,#method)

#define OverloadedMethodOld( mock , method, prototype ) \
	mock[prototype::MemberType<std::remove_reference<decltype(mock.get())>::type>::get(&std::remove_reference<decltype(mock.get())>::type::method)].setMethodDetails(#mock,#method)

#define OverloadedMethod( mock , method, prototype ) \
	mock[fakeit::Prototype<prototype>::MemberType<std::remove_reference<decltype(mock.get())>::type>::get(&std::remove_reference<decltype(mock.get())>::type::method)].setMethodDetails(#mock,#method)

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



#pragma once

#ifdef _MSC_VER
#define __func__ __FUNCTION__
#endif

#define MOCK_TYPE(mock) \
    std::remove_reference<decltype(mock.get())>::type

#define OVERLOADED_METHOD_PTR(mock, method, prototype) \
    fakeit::Prototype<prototype>::MemberType<MOCK_TYPE(mock)>::get(&MOCK_TYPE(mock)::method)

#define CONST_OVERLOADED_METHOD_PTR(mock, method, prototype) \
    fakeit::Prototype<prototype>::MemberType<MOCK_TYPE(mock)>::getconst(&MOCK_TYPE(mock)::method)

#define Dtor(mock) \
    mock.dtor().setMethodDetails(#mock,"destructor")

#define Method(mock, method) \
    mock.template stub<__COUNTER__>(&MOCK_TYPE(mock)::method).setMethodDetails(#mock,#method)

#define OverloadedMethod(mock, method, prototype) \
    mock.template stub<__COUNTER__>(OVERLOADED_METHOD_PTR( mock , method, prototype )).setMethodDetails(#mock,#method)

#define ConstOverloadedMethod(mock, method, prototype) \
    mock.template stub<__COUNTER__>(CONST_OVERLOADED_METHOD_PTR( mock , method, prototype )).setMethodDetails(#mock,#method)

#define Verify(...) \
        Verify( __VA_ARGS__ ).setFileInfo(__FILE__, __LINE__, __func__)

#define Using(...) \
        Using( __VA_ARGS__ )

#define VerifyNoOtherInvocations(...) \
    VerifyNoOtherInvocations( __VA_ARGS__ ).setFileInfo(__FILE__, __LINE__, __func__)

#define Fake(...) \
    Fake( __VA_ARGS__ )

#define When(call) \
    When(call)


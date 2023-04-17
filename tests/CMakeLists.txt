add_executable(FakeIt_tests
    argument_matching_tests.cpp
    cpp14_tests.cpp
    custom_event_formatting_tests.cpp
    custom_testing_framework_tests.cpp
    default_behaviore_tests.cpp
    default_event_formatting_tests.cpp
    dtor_mocking_tests.cpp
    event_notification_tests.cpp
    functional.cpp
    gcc_stubbing_multiple_values_tests.cpp
    gcc_type_info_tests.cpp
    miscellaneous_tests.cpp
    move_only_return_tests.cpp
    msc_stubbing_multiple_values_tests.cpp
    msc_type_info_tests.cpp
    overloadded_methods_tests.cpp
    referece_types_tests.cpp
    remove_const_volatile_tests.cpp
    rvalue_arguments_tests.cpp
    sequence_verification_tests.cpp
    spying_tests.cpp
    streaming_tests.cpp
    stubbing_tests.cpp
    tpunit++main.cpp
    type_info_tests.cpp
    verification_errors_tests.cpp
    verification_tests.cpp
    VirtualOffsetSelectorTest.cpp
)

target_link_libraries(FakeIt_tests PRIVATE
    FakeIt_dev
    FakeIt_config_standalone
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "^(Apple)?Clang$")
    target_compile_options(FakeIt_tests PRIVATE -Wall -Wextra -Wno-ignored-qualifiers -O0 -g3)
elseif(MSVC)
    target_compile_options(FakeIt_tests PRIVATE /W3 /sdl /Od)
endif()

if(OVERRIDE_CXX_STANDARD_FOR_TESTS)
    set_target_properties(FakeIt_tests PROPERTIES CXX_STANDARD ${OVERRIDE_CXX_STANDARD_FOR_TESTS})
    message(STATUS "Building tests in C++${OVERRIDE_CXX_STANDARD_FOR_TESTS}.")
endif()

if(ENABLE_SANITIZERS_IN_TESTS)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "^(Apple)?Clang$")
        target_compile_options(FakeIt_tests PRIVATE -fsanitize=address,undefined -fno-sanitize-recover=address,undefined)
        target_link_options(FakeIt_tests PRIVATE -fsanitize=address,undefined -fno-sanitize-recover=address,undefined)
        target_compile_definitions(FakeIt_tests PRIVATE FAKEIT_DISABLE_UBSAN_TRIGGERING_TESTS)
        message(STATUS "Sanitizers enabled: address, undefined.")
    else()
        message(SEND_ERROR "Sanitizers requested but compiler is not compatible.")
    endif()
endif()

if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "^(Apple)?Clang$")
        target_compile_options(FakeIt_tests PRIVATE --coverage)
        target_link_options(FakeIt_tests PRIVATE --coverage)
        message(STATUS "Code coverage enabled.")
    else()
        message(SEND_ERROR "Code coverage requested but compiler is not compatible.")
    endif()
endif()

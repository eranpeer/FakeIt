# ARCH_INDEPENDENT option at write_basic_package_version_file requires 3.14 version of CMake.
cmake_minimum_required(VERSION 3.14...3.26)

project(FakeIt VERSION 2.4.1 LANGUAGES CXX)

option(ENABLE_TESTING "Enable build of tests." OFF)
set(OVERRIDE_CXX_STANDARD_FOR_TESTS "" CACHE STRING "Override the C++ standard used for building tests.")
option(WARNINGS_AS_ERRORS_FOR_TESTS "Treat warnings as errors when building tests." OFF)
option(ENABLE_SANITIZERS_IN_TESTS "Enable address / undefined sanitizers in tests." OFF)
option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang." OFF)

# Directory containing main targets of FakeIt.
add_subdirectory(include)

# Directory containing config targets of FakeIt.
add_subdirectory(config)

if(ENABLE_TESTING)
    enable_testing()
    # Directory containing test targets of FakeIt.
    add_subdirectory(tests)
endif()

# Directory containing single header targets of FakeIt.
add_subdirectory(single_header)

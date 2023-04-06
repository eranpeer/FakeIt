# ARCH_INDEPENDENT option at write_basic_package_version_file requires 3.14 version of CMake.
cmake_minimum_required(VERSION 3.14)

project(FakeIt VERSION 2.3.2 LANGUAGES CXX)

option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang." OFF)
option(ENABLE_TESTING "Enable building unit tests" ON)

# Directory containing main targets of FakeIt.
add_subdirectory(include)

# Directory containing config targets of FakeIt.
add_subdirectory(config)

# Directory containing test targets of FakeIt.
if(${ENABLE_TESTING})
  add_subdirectory(tests)
endif()

# Directory containing single header targets of FakeIt.
add_subdirectory(single_header)

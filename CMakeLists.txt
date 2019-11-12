
cmake_minimum_required(VERSION 3.0)

project(FakeIt VERSION 2.0.5 LANGUAGES CXX)

#Create library with INTERFACE parameter because we want to use headers from outside.
add_library(FakeIt INTERFACE)

#Add include directories to the interface library.
#If we use FakeIt from build-tree then we use BUILD_INTERFACE wrapper.
#If we use FakeIt from install-tree then we use INSTALL_INTERFACE wrapper.
target_include_directories(FakeIt INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/single_header/standalone>
    $<INSTALL_INTERFACE:include>
)

#Add alias for the library with namespace.
add_library(FakeIt::FakeIt ALIAS FakeIt)

#Install single_header file to install directory to include subdirectory.
#Trailing slash at "single_header/standalone/" string means that
#the header will be installed like ${CMAKE_INSTALL_PREFIX}/include/fakeit.hpp
#instead of ${CMAKE_INSTALL_PREFIX}/include/standalone/fakeit.hpp.
install(DIRECTORY single_header/standalone/ DESTINATION include)

#Add FakeIt target to the FakeItConfig.cmake file
#which will be used to find FakeIt from outside.
install(TARGETS FakeIt EXPORT FakeItConfig)

#Install FakeItConfig.cmake file to the ${CMAKE_INSTALL_PREFIX}/share/FakeIt/cmake directory
#and all correlated targets should be called from outside by the namespace.
install(EXPORT FakeItConfig NAMESPACE FakeIt:: DESTINATION share/FakeIt/cmake)

#Add version control.
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_BINARY_DIR}/FakeItConfigVersion.cmake"
    VERSION
        ${PROJECT_VERSION}
    COMPATIBILITY
        AnyNewerVersion
)
install(FILES "${PROJECT_BINARY_DIR}/FakeItConfigVersion.cmake" DESTINATION share/FakeIt/cmake)


cmake_minimum_required(VERSION 2.8.4)

include_directories ("../../include" "../../config/gtest" "../tested_app" "C:/Users/eran/Desktop/tools/gtest-1.7.0/include")

link_directories("C:/Users/eran/Desktop/tools/gtest-1.7.0/")

#add_subdirectory(sources)

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/Users/eran/Desktop/tools/gtest-1.7.0/")

project(tests)

set(CMAKE_CXX_FLAGS "-std=c++11")

file(GLOB SOURCE_FILES sources/*.cpp)

add_executable(fakeit_with_gtest ${SOURCE_FILES})
target_link_libraries(fakeit_with_gtest gtest_main)
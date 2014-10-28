cmake_minimum_required(VERSION 2.8.4)
include_directories ("../../include" "../../config/tpunit" "../tested_app")
project(tests)

set(CMAKE_CXX_FLAGS "-std=c++11")

file(GLOB SOURCE_FILES *.cpp sources/*.cpp)

add_executable(fakeit_with_tpunit ${SOURCE_FILES})
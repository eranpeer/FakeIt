cmake_minimum_required(VERSION 2.8.4)
include_directories ("../include")
project(tests)

set(CMAKE_CXX_FLAGS "-std=c++11")

file(GLOB SOURCE_FILES *.cpp ../include/mockutils/*.hpp ../include/fakeit/*.hpp ../include/*.hpp)

add_executable(tests ${SOURCE_FILES})
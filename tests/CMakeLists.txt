cmake_minimum_required(VERSION 2.8.4)
include_directories ("../include" "../config/standalone")
project(tests)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wno-ignored-qualifiers -pedantic -O3 -flto -Wl,--allow-multiple-definition")

file(GLOB SOURCE_FILES *.cpp ../include/mockutils/*.hpp ../include/fakeit/*.hpp ../include/*.hpp)

add_executable(tests ${SOURCE_FILES})

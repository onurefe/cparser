cmake_minimum_required(VERSION 3.8)
project(flashsimulator VERSION 0.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

include(CTest)
enable_testing()

add_subdirectory(googletest)
add_library(cparser cparser/src/cparser.c cparser/inc/cparser.h)
add_subdirectory(tests)

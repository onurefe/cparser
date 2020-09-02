cmake_minimum_required(VERSION 3.8)
project(cparsertest VERSION 0.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

include(CTest)

enable_testing()

add_executable(cparsertest test.cpp)
target_link_libraries(cparsertest PUBLIC gtest_main cparser)

add_test(NAME cparsertest COMMAND cparsertest)
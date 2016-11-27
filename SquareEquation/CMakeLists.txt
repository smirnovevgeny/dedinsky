cmake_minimum_required(VERSION 3.6)
project(SquareEquation)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES SquareEquation.c)
add_executable(SquareEquation ${SOURCE_FILES})
find_library(M_LIB m)
target_link_libraries(SquareEquation ${M_LIB})
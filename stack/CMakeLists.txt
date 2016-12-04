cmake_minimum_required(VERSION 3.6)
project(stack)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES stack.c)
add_executable(stack ${SOURCE_FILES})
add_executable(stack.c)
add_executable(processor processor.c)
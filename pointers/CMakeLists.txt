cmake_minimum_required(VERSION 3.6)
project(pointers)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#set(SOURCE_FILES input.c)
#add_executable(pointers ${SOURCE_FILES})
add_executable(input input.c)
add_executable(str str.c)
add_executable(onegin onegin.c)
add_executable(exper exper.c)
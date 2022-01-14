cmake_minimum_required(VERSION 3.0.0)
project(wolf VERSION 0.1.0)

include_directories(${CMAKE_SOURCE_DIR})

add_library(wolf wolf.cpp)
target_link_libraries(wolf utils)

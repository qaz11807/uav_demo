cmake_minimum_required(VERSION 3.0.0)
project(utils VERSION 0.1.0)

include_directories(${CMAKE_SOURCE_DIR})

include(CTest)
enable_testing()

add_library(utils utils.cpp)
target_link_libraries(utils graph)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

cmake_minimum_required(VERSION 3.0.0)
project(gwo VERSION 0.1.0)

add_subdirectory(wolf)
include_directories(${CMAKE_SOURCE_DIR})

include(CTest)
enable_testing()

add_library(gwo gwo.cpp)
target_link_libraries(gwo wolf)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

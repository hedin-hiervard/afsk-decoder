cmake_minimum_required(VERSION 3.10)

project(afsk_decoder)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(lib/gtest)

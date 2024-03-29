cmake_minimum_required(VERSION 3.10)

project(afsk_decoder)

set(BIN_DIR ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(
	src
	lib
	lib/gtest/googlemock/include
	lib/argparse/include
)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(lib/gtest)

SET_TARGET_PROPERTIES(gmock PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}
)

SET_TARGET_PROPERTIES(gtest PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}
)

SET_TARGET_PROPERTIES(gmock_main PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}
)

SET_TARGET_PROPERTIES(gtest_main PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}
)
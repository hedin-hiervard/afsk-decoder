cmake_minimum_required(VERSION 3.10)

project(afsk_decoder)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(afsk_decoder
	src/decode.cpp
	src/ZeroCrossingDetector.cpp
)

target_include_directories(afsk_decoder
	PUBLIC
    	lib/audiofile
)

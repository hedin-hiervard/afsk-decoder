set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCE_FILES
	main.cpp
	ZeroCrossingDetector.cpp
	BitDetector.cpp
	ByteDetector.cpp
	MessageDecoder.cpp
)

add_executable(${BINARY}_run ${SOURCE_FILES}
)

add_library(${BINARY}_lib STATIC ${SOURCE_FILES})
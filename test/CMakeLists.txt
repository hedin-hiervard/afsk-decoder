set(BINARY ${CMAKE_PROJECT_NAME}_test)

set(TEST_FILES
	main.cpp
	ZeroCrossingDetector_Test.cpp
)

add_executable(${BINARY} ${TEST_FILES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)
add_executable(tests tests.cpp)

target_include_directories(tests PUBLIC ${DOCTEST_INCLUDE_DIR})

target_link_libraries(tests PRIVATE )

add_test(NAME tests COMMAND tests)
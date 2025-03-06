add_executable(test_date_utils tests.cpp)

target_include_directories(test_date_utils PUBLIC ${DOCTEST_INCLUDE_DIR})

target_link_libraries(test_date_utils PRIVATE date_utils)

add_test(NAME test_date_utils COMMAND test_date_utils)
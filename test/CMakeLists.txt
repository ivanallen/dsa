include_directories(SYSTEM ${GTEST_INCLUDE_DIR})

function(dsa_test test_name)
    set(TEST_EXECUTABLE run_${test_name})
    set(TEST_SOURCE ${test_name}.cpp)

    add_executable(${TEST_EXECUTABLE} ${TEST_SOURCE})
    add_dependencies(${TEST_EXECUTABLE} googletest_ep)
    target_link_libraries(${TEST_EXECUTABLE} ${GTEST_STATIC_LIB} ${GTEST_MAIN_STATIC_LIB} ${GMOCK_MAIN_STATIC_LIB} pthread)
    add_test(${test_name} ${TEST_EXECUTABLE})
endfunction()

dsa_test(binary_tree_test)
dsa_test(vector_test)

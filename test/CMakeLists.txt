include_directories(SYSTEM ${GTEST_INCLUDE_DIR})

add_executable(demo_test demo_test.cpp)
add_dependencies(demo_test dsa googletest_ep)
target_link_libraries(demo_test ${GTEST_STATIC_LIB} ${GTEST_MAIN_STATIC_LIB} ${GMOCK_MAIN_STATIC_LIB} pthread)

add_executable(binary_tree_test binary_tree_test.cpp)
add_dependencies(binary_tree_test dsa googletest_ep)
target_link_libraries(binary_tree_test ${GTEST_STATIC_LIB} ${GTEST_MAIN_STATIC_LIB} ${GMOCK_MAIN_STATIC_LIB} pthread)

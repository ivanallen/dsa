add_executable(demo_test demo_test.cpp)
add_dependencies(demo_test dsa googletest_ep)
include_directories(SYSTEM ${GTEST_INCLUDE_DIR})
target_link_libraries(demo_test ${GTEST_STATIC_LIB} ${GTEST_MAIN_STATIC_LIB} ${GMOCK_MAIN_STATIC_LIB} pthread)
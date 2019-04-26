cmake_minimum_required(VERSION 3.11)
project(dsa)

set(CMAKE_CXX_STANDARD 11)

set( CMAKE_BUILD_TYPE "Debug")

include_directories(include)

set(DSA_SRC_FILES
#        "src/kmp.cpp"
        "src/lcs.cpp")


add_library(dsa SHARED ${DSA_SRC_FILES})

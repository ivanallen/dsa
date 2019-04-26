cmake_minimum_required(VERSION 3.13)
project(dsa)

set(CMAKE_CXX_STANDARD 11)

include_directories(b-tree)
include_directories(binary-search-tree)
include_directories(binary-tree)
include_directories(binary_tree)
include_directories(graph)
include_directories(hashmap)
include_directories(heap)
include_directories(list)
include_directories(lru)
include_directories(monoqueue)
include_directories(red-black-tree)
include_directories(skiplist)
include_directories(sort)
include_directories(unionfind)

add_executable(dsa
        include/btree.h
        test/btree_test.cpp
        include/binary_search_tree.h
        test/binary_search_tree_test.cpp
        include/binary_tree.h
        include/binary_tree.h
        test/binary_tree_test.cpp
        include/graph.h
        test/graph_test.cpp
        test/hash_map_test.cpp
        include/hashmap.h
        include/heap.h
        test/heap_test.cpp
        include/list.h
        test/list_test.cpp
        include/lru.h
        test/lru_test.cpp
        include/monoqueue.h
        test/monoqueue_test.cc
        include/rb_tree.h
        test/rb_tree_test.cpp
        src/media.cpp
        include/skiplist.h
        test/skiplist_test.cpp
        include/quick_sort.h
        test/quick_sort_test.cpp
        src/kmp.cpp
        src/lcs.cpp
        src/manacher.cpp
        include/unionfind.h
        test/unionfind_test.cpp
        README.md)

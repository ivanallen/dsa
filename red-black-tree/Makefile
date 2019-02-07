DEBUG=_

rb_tree_test: rb_tree_test.cpp ../red-black-tree/rb_tree.h ../binary-search-tree/binary_search_tree.h ../binary-tree/binary_tree.h
	g++ -g -std=c++17 -D${DEBUG} -o $@ -I.. rb_tree_test.cpp

clean:
	rm -r rb_tree_test rb_tree_test.dSYM

.PHONY: clean

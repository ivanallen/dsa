/*
 * Copyright (c) 2019, DSA authors
 * All rights reserved.
 * Authors: Allen
 */

#include <vector>
#include <optional>
#include <red_black_tree.h>

using namespace dsa;

using Tree = RedBlackTree<int, int>;

std::ostream& operator<<(std::ostream& out, const std::pair<int, int>& val) {
    return out << "(" << val.first << "," << val.second << ")";
}

int main() {
    Tree tree;

    for (int i = 10; i <= 100; i += 10) {
        tree.insert(i, 2*i);
    }
    for (int i = 5; i <= 95; i += 10) {
        tree.insert(i, 2*i);
    }
    std::cout << tree << std::endl << std::endl;


    for (int i = 5; i <= 100; i += 5) {
        tree.remove(i);
        std::cout << "Remove:" << i << std::endl;
        std::cout << tree << std::endl << std::endl;
    }
    return 0;
}

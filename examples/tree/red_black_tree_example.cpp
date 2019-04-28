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
    tree.insert(50, 200);
    tree.insert(100, 200);
    tree.insert(150, 300);
    tree.insert(200, 400);
    tree.insert(20, 40);
    tree.insert(60, 120);
    tree.insert(10, 20);
    tree.insert(70, 140);
    std::cout << tree << std::endl << std::endl;
    return 0;
}

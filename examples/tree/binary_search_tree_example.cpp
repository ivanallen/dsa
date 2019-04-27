/*
 * Copyright (c) 2019, DSA authors
 * All rights reserved.
 * Authors: Allen
 */

#include <vector>
#include <optional>
#include <binary_search_tree.h>

using namespace dsa;

using Tree = BinarySearchTree<int, int>;

std::ostream& operator<<(std::ostream& out, const std::pair<int, int>& val) {
    return out << "(" << val.first << "," << val.second << ")";
}

int main() {
    std::vector<std::optional<int>> values = {15, 6, 18, 3, 8, 17, 20, 2, 4, {}, 13, 
            {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, 9};
    std::vector<std::optional<std::pair<int, int>>> nodes;

    for (auto& e : values) {
        if (e) {
            nodes.emplace_back(std::make_pair(*e, *e * 2));
        } else {
            nodes.emplace_back(std::nullopt);
        }
    }

    Tree tree(nodes);

    const std::string tree_graph = R"(  
                           15
                        /     \
                      /         \
                     6           18
                   /   \       /    \
                  3     8     17    20
                /   \     \
               2     4     13
                          /
                         9
    )"; 

    std::cout << tree_graph << std::endl;
    std::cout << tree << std::endl << std::endl;
    std::cout << "Find 15: the value is " << tree.search(15).value_or(-1) << std::endl;

    try {
        std::cout << "15's successor is " << tree.successor(15).value() << std::endl;
        std::cout << "15's predecessor is " << tree.predecessor(15).value() << std::endl;
        std::cout << "13's successor is " << tree.successor(13).value() << std::endl;
        std::cout << "13's predecessor is " << tree.predecessor(13).value() << std::endl << std::endl;
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    tree.left_rotate(15);
    std::cout << tree << std::endl << std::endl;
    return 0;
}

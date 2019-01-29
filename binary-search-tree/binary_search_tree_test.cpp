#include "binary_search_tree.h"
#include <string>
#include <iostream>

using BST = BinarySearchTree<int, int>;
BST create(std::vector<std::optional<int>>&& arr) {
    std::vector<std::optional<std::pair<int, int>>> list;
    for (auto key : arr) {
        if (key) {
            list.emplace_back(std::make_pair(*key, *key));
        } else {
            list.emplace_back(std::nullopt);
        }
    }
    BST bst(std::move(list));
    return std::move(bst);
}

int main() {
    BST bst = create({15, 6, 18, 3, 7, 17, 20, 2, 4, {}, 13, 
            {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, 9});

    std::string tree = R"(  
                           15
                        /     \
                      /         \
                     6           18
                   /   \       /    \
                  3     7     17    20
                /   \     \
               2     4     13
                          /
                         9
    )"; 

    std::cout << tree << std::endl;

    // inorder
    std::cout << "inorder:";
    bst.inorder([](const int& key, const int& value) {
        std::cout << ' ' << key;
    });
    std::cout << std::endl;

    int compare_count = 0;

    // search
    auto node = bst.search(20, &compare_count);
    if (node != nullptr) {
        std::cout << "search: key = " << node->key << "; value = " << node->value << std::endl;
    }
    std::cout << "search compare_count:" << compare_count << std::endl;

    // iterative search
    node = bst.iterative_search(20, &compare_count);
    if (node != nullptr) {
        std::cout << "iterative search: key = " << node->key << "; value = " << node->value << std::endl;
    }
    std::cout << "iterative compare_count:" << compare_count << std::endl;

    // minimum
    node = bst.minimum();
    if (node != nullptr) {
        std::cout << "minimum: key = " << node->key << "; value = " << node->value << std::endl;
    }

    // maximum
    node = bst.maximum();
    if (node != nullptr) {
        std::cout << "maximum: key = " << node->key << "; value = " << node->value << std::endl;
    }
    return 0;
}

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
    BST bst = create({15, 6, 18, 3, 8, 17, 20, 2, 4, {}, 13, 
            {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, 9});

    std::string tree = R"(  
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

    std::cout << tree << std::endl;

    // inorder
    std::cout << "inorder:";
    bst.inorder([](const int& key, const int& value) {
        std::cout << ' ' << key;
    });
    std::cout << std::endl;

    int compare_count = 0;

    // search
    auto val = bst.search(20, &compare_count);
    std::cout << "search: value=" << val.value_or(-1) << ' ';
    std::cout << "compare_count=" << compare_count << std::endl;

    // iterative search
    val = bst.iterative_search(20, &compare_count);
    std::cout << "iterative search: value=" << val.value_or(-1) << ' ';
    std::cout << "compare_count=" << compare_count << std::endl;

    val = bst.iterative_search(8, &compare_count);
    std::cout << "iterative search: value=" << val.value_or(-1) << ' ';
    std::cout << "compare_count=" << compare_count << std::endl;

    // minimum
    auto pair = bst.minimum();
    if (pair)
        std::cout << "minimum: key=" << pair->first<< " value=" << pair->second<< std::endl;

    // maximum
    pair = bst.maximum();
    if (pair)
        std::cout << "minimum: key=" << pair->first << " value=" << pair->second<< std::endl;


    // insert
    bst.insert(7, 7);

    std::cout << "insert 7, inorder:";
    bst.inorder([](const int& key, const int& value) {
        std::cout << ' ' << key;
    });
    std::cout << std::endl;

    // remove 
    bst.remove(6);
    std::cout << "remove 6, inorder:";
    bst.inorder([](const int& key, const int& value) {
        std::cout << ' ' << key;
    });
    std::cout << std::endl;
    return 0;
}

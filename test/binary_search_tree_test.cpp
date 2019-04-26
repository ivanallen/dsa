#include "../include/binary_search_tree.h"
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

void show(const BST& bst, const char* prompt = nullptr) {
    if (prompt != nullptr)
        std::cout << prompt << ' ';
    std::cout << "inorder:[";
    bst.inorder([](const int& key, const int& value, Color color) {
        std::cout << ' ' << key;
    });
    std::cout << " ] preorder:[";
    bst.preorder([](const int& key, const int& value, Color color) {
        std::cout << ' ' << key;
    });
    std::cout << " ] postorder:[";
    bst.postorder([](const int& key, const int& value, Color color) {
        std::cout << ' ' << key;
    });
    std::cout << " ]" << std::endl;
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

    show(bst);
    bst.left_rotate(6);
    show(bst);
    bst.right_rotate(8);
    show(bst);

    // 将 2 叉树旋转成向右延展的链
    /*
    bst.left_rotate(3);
    bst.right_rotate(13);
    bst.right_rotate(18);
    bst.left_rotate(6);
    bst.left_rotate(8);
    bst.left_rotate(9);
    bst.left_rotate(13);

    bst.right_rotate(15);
    bst.right_rotate(13);
    bst.right_rotate(9);
    bst.right_rotate(8);
    bst.right_rotate(6);
    bst.right_rotate(4);
    bst.right_rotate(4);
    bst.right_rotate(3);
    bst.right_rotate(2);
    show(bst, "extent link");
    */


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
    show(bst, "insert 7");

    // remove 
    bst.remove(6);
    show(bst, "remove 6");
    return 0;
}

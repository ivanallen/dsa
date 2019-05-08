/*
 * Copyright (c) 2019, ivan_allen@163.com 
 * All rights reserved.
 */

#include <binary_tree.h>

using namespace dsa;

using Tree = BinaryTree<int>;

int main() {
    Tree tree({15, 6, 18, 3, 8, 17, 20, 2, 4, {}, 13, 
            {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, 9});

    std::string tree_graph = R"(  
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

    bool first = true;
    std::cout << "postorder:[";
    tree.postorder([&first](int e) {
        if (first) {
            std::cout << e;
            first = false;
        } else {
            std:: cout << "," << e;
        }
    });

    std::cout << "]" << std::endl;
    std::cout << std::endl;

    // 经过旋转后，变成向左延展的链
    tree.left_rotate(3);
    tree.right_rotate(13);
    tree.right_rotate(18);

    tree.left_rotate(6);
    tree.left_rotate(8);
    tree.left_rotate(9);
    tree.left_rotate(15);
    tree.left_rotate(17);
    tree.left_rotate(18);

    std::cout << tree << std::endl << std::endl;
}

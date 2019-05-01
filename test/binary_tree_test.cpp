#include <string_view>
#include <sstream>
#include <gtest/gtest.h>
#include "binary_tree.h"

namespace dsa {
namespace test {

using Tree = BinaryTree<int>;

TEST(BinaryTreeTest, BuildTreeEmpty) {
    Tree tree;
    const std::string expect = "preorder:[]\ninorder: []";
    EXPECT_EQ(tree.dump(false), expect);
}

TEST(BinaryTreeTest, BuildTreeOneNode) {
    Tree tree({100});
    const std::string expect = "preorder:[100]\ninorder: [100]";
    EXPECT_EQ(tree.dump(false), expect);
}

TEST(BinaryTreeTest, BuildTreeTwoNodes) {
    Tree tree({5, 10});
    const std::string expect = "preorder:[5,10]\ninorder: [10,5]";
    EXPECT_EQ(tree.dump(false), expect);
}

TEST(BinaryTreeTest, BuildTreeThreeNodes1) {
    Tree tree({5, 10, 15});
    const std::string expect = "preorder:[5,10,15]\ninorder: [10,5,15]";
    EXPECT_EQ(tree.dump(false), expect);
}

TEST(BinaryTreeTest, BuildTreeThreeNodes2) {
    Tree tree({5, 10, {}, 15});
    const std::string expect = "preorder:[5,10,15]\ninorder: [15,10,5]";
    EXPECT_EQ(tree.dump(false), expect);
}

TEST(BinaryTreeTest, BuildTreeThreeNodes3) {
    // 15 can't be inserted into tree
    Tree tree({5, {}, 10, 15});
    const std::string expect = "preorder:[5,10]\ninorder: [5,10]";
    EXPECT_EQ(tree.dump(false), expect);
}

TEST(BinaryTreeTest, BuildTreeThreeNodes4) {
    // All nodes can't be inserted into tree
    Tree tree({{}, 5, 10, 15});
    const std::string expect = "preorder:[]\ninorder: []";
    EXPECT_EQ(tree.dump(false), expect);
}

TEST(BinaryTreeTest, BuildTree) {
    constexpr std::string_view tree_graph = R"(
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

    Tree tree({15, 6, 18, 3, 8, 17, 20, 2, 4, {}, 13, 
            {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, 9});

    std::string dump = tree.dump(false);

    const std::string expect = "preorder:[15,6,3,2,4,8,13,9,18,17,20]\n"
        "inorder: [2,3,4,6,8,9,13,15,17,18,20]";

    EXPECT_EQ(dump, expect);
}

TEST(BinaryTreeTest, LeftRotate) {
    constexpr std::string_view tree_graph = R"(
                  2         <- left_rotate
                /   \
               4     1
                   /   \
                  5     3

                 ||

                  1
                /   \
               2     3
             /   \
            4     5
    )";
    Tree tree({2,4,1,{},{},5,3});
    tree.left_rotate(2);
    const std::string expect = "preorder:[1,2,4,5,3]\ninorder: [4,2,5,1,3]";
    EXPECT_EQ(tree.dump(false), expect);
}

TEST(BinaryTreeTest, RightRotate) {
    constexpr std::string_view tree_graph = R"(
                  1        <- right_rotate
                /   \
               2     3
             /   \
            4     5

                  ||

                  2
                /   \
               4     1
                   /   \
                  5     3
    )";
    Tree tree({1, 2, 3, 4, 5});
    tree.right_rotate(1);
    const std::string expect = "preorder:[2,4,1,5,3]\ninorder: [4,2,5,1,3]";
    EXPECT_EQ(tree.dump(false), expect);
}

TEST(BinaryTreeTest, Rotate) {
    constexpr std::string_view tree_graph = R"(
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

    Tree tree({15, 6, 18, 3, 8, 17, 20, 2, 4, {}, 13,
            {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, 9});

    tree.left_rotate(3);
    tree.right_rotate(13);
    tree.right_rotate(18);

    tree.left_rotate(6);
    tree.left_rotate(8);
    tree.left_rotate(9);
    tree.left_rotate(15);
    tree.left_rotate(17);
    tree.left_rotate(18);

    std::string dump = tree.dump(false);
    const std::string expect = "preorder:[20,18,17,15,13,9,8,6,4,3,2]\n"
        "inorder: [2,3,4,6,8,9,13,15,17,18,20]";

    EXPECT_EQ(dump, expect);
}

} // namespace test
} // namespace dsa

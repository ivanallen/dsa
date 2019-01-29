#pragma once

#include <memory>
#include <queue>
#include <vector>
#include <optional>
#include <iostream>

#include <binary-tree/binary_tree.h>

template <typename K, typename V>
class BinarySearchTree : public BinaryTree<K, V> {
public:
    //using BinaryTree<K, V>::root;
    using typename BinaryTree<K, V>::TreeNode;

    BinarySearchTree(std::vector<std::optional<std::pair<K, V>>>&& list) :
        BinaryTree<K, V>(std::move(list)) {}

    // 查询 key
    TreeNode* search(const K& k, int* compare_count = nullptr) {
        if (compare_count != nullptr) {
            *compare_count = 0;
        }
        return _search(this->root(), k, compare_count);
    }

    TreeNode* iterative_search(const K& k, int* compare_count = nullptr) {
        if (compare_count != nullptr) {
            *compare_count = 0;
        }

        TreeNode* x = this->root();
        while(x != nullptr) {
            if (k == x->key) {
                if (compare_count != nullptr) {
                    (*compare_count) += 1;
                }
                break;
            } else if (k < x->key) {
                if (compare_count != nullptr) {
                    (*compare_count) += 2;
                }
                x = x->left.get();
            } else {
                if (compare_count != nullptr) {
                    (*compare_count) += 2;
                }
                x = x->right.get();
            }
        }
        return x;
    }
    
    TreeNode* minimum() {
        TreeNode* x = this->root();
        if (x == nullptr) return x;

        // 假设 x 总不为 null
        while(x->left != nullptr) {
            x = x->left.get();
        }
        return x;
    }

    TreeNode* maximum() {
        TreeNode* x = this->root();
        if (x == nullptr) return x;

        // 假设 x 总不为 null
        while(x->right!= nullptr) {
            x = x->right.get();
        }
        return x;
    }
private:
    TreeNode* _search(TreeNode* x, const K& k, int* compare_count) {
        if (x == nullptr || x->key == k) {
            if (compare_count != nullptr) {
                ++(*compare_count);
            }
            return x;
        }

        if (compare_count != nullptr) {
            // 一次相等比较，一次 < 比较
            (*compare_count) += 2;
        }

        if (k < x->key)
            return _search(x->left.get(), k, compare_count);
        else
            return _search(x->right.get(), k, compare_count);
    }
};

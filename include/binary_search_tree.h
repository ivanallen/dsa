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
    using typename BinaryTree<K, V>::TreeNode;

    BinarySearchTree() {}

    BinarySearchTree(std::vector<std::optional<std::pair<K, V>>>&& list) :
        BinaryTree<K, V>(std::move(list)) {}

    // 递归算法查询 key
    std::optional<V> search(const K& k, int* compare_count = nullptr) {
        if (compare_count != nullptr) {
            *compare_count = 0;
        }
        auto x = _search(this->root(), k, compare_count);
        return this->is_leaf(x) ? std::nullopt : std::optional<V>(x->value);
    }

    // 迭代算法查询
    std::optional<V> iterative_search(const K& k, int* compare_count = nullptr) {
        if (compare_count != nullptr) {
            *compare_count = 0;
        }

        TreeNode* x = this->root();
        while(!this->is_leaf(x)) {
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
        return this->is_leaf(x) ? std::nullopt : std::optional<V>(x->value);
    }
    
    // 求最小
    std::optional<std::pair<K, V>> minimum() {
        TreeNode* x = this->root();
        if (this->is_leaf(x)) return std::nullopt;

        auto node = _minimum(x);
        return std::make_pair(node->key, node->value);
    }

    // 求最大
    std::optional<std::pair<K, V>> maximum() {
        TreeNode* x = this->root();
        if (this->is_leaf(x)) return std::nullopt;

        auto node = _maximum(x);
        return std::make_pair(node->key, node->value);
    }

    void insert(const K& key, const V& value) {
        auto z = std::make_unique<TreeNode>(key, value);
        _insert(z.release());
    }

    bool remove(const K& k) {
        TreeNode* z = _search(this->root(), k, nullptr);
        if (this->is_leaf(z)) return false;

        return _remove(z);
    }

    // 测试用
    bool left_rotate(const K& k) {
        auto x = _search(this->root(), k, nullptr);
        return BinaryTree<K, V>::left_rotate(x);
    }

    // 测试用
    bool right_rotate(const K& k) {
        auto x = _search(this->root(), k, nullptr);
        return BinaryTree<K, V>::right_rotate(x);
    }

protected:
    TreeNode* _search(TreeNode* x, const K& k, int* compare_count) {
        if (this->is_leaf(x) || x->key == k) {
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

    TreeNode* _minimum(TreeNode* x) {
        // 假设 x 总不为 null
        while(!this->is_leaf(x->left.get())) {
            x = x->left.get();
        }
        return x;
    }

    TreeNode* _maximum(TreeNode* x) {
        // 假设 x 总不为 null
        while(!this->is_leaf(x->right.get())) {
            x = x->right.get();
        }
        return x;
    }

    // 约定 z 是新插入的节点
    void _insert(TreeNode* z) {
        // 目标，搜寻 z 应该插入的位置
        // 技巧：维护一个指针 y, y 始终是 x 的父节点
        auto x = this->root();
        // 1. 空树
        if (this->is_leaf(x)) {
            this->set_root(z);
            return;
        }
        TreeNode* y = nullptr;

        // 2. 非空，寻找 z 的位置
        while(!this->is_leaf(x)) {
            y = x;

            // down
            if (z->key < y->key) {
                x = x->left.get();
            } else {
                x = x->right.get();
            }
        }

        if (z->key < y->key) {
            y->left.reset(z);
        } else {
            y->right.reset(z);
        }
        z->p = y;
    }

    // 求后继
    TreeNode* _successor(TreeNode* x) {
        if (this->is_leaf(x)) return nullptr;

        // 1. right child is not empty
        if (!this->is_leaf(x->right.get())) {
            return _minimum(x->right.get());
        }

        // 2. right child is empty, find lowest acestor
        TreeNode* y = x->p;
        while(!this->is_leaf(y) && y->right.get() == x) {
            x = y;
            y = x->p;
        }
        return y;
    }

    bool _remove(TreeNode* z) {
        if (this->is_leaf(z)) return false;

        if (this->is_leaf(z->left.get())) {
            auto r = z->right.get();
            this->detach(r);
            this->transplant(z, r);
        } else if (this->is_leaf(z->right.get())) {
            auto l = z->right.get();
            this->detach(l);
            this->transplant(z, l);
        } else {
            // 既有左孩子，又有右孩子
            // 1. 找 z 后继 y
            auto y = _successor(z);
            if (y->p == z) {
                // 如果后继就是 z 的右孩子
                // 先摘除后继
                this->detach(y);
            } else {
                // 先将后继的右孩子 x，替换掉后继
                auto x = y->right.get();
                // 后继的右孩子摘下
                this->detach(x);
                // 后继的右孩子 x 替换掉后继，后继被摘下
                this->transplant(y, x);
                // 将 z 的右孩子挂到后继的右孩子上
                auto r = z->right.get();
                this->detach(r);
                y->right.reset(r);
                r->p = y;
            }

            this->transplant(z, y);
            auto l = z->left.get();
            this->detach(l);
            // 将 z 以前的左孩子挂到 y 的左孩子, y 左孩子一定是空
            y->left.reset(l);
            l->p = y;
        }

        delete z;
        return true;
    }
};

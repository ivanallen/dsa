/*
 * Copyright (c) 2019, DSA authors.
 * All rights reserved.
 * Authors: Allen
 */

// 二叉搜索树类
#pragma once

#include <memory>
#include <queue>
#include <vector>
#include <optional>
#include <iostream>

#include <binary_tree.h>

namespace dsa {

template <typename K, typename V>
class BinarySearchTree : public BinaryTree<std::pair<K, V>> {
public:
    using Node = typename BinaryTree<std::pair<K, V>>::Node;
    
    // https://stackoverflow.com/questions/4643074/why-do-i-have-to-access-template-base-class-members-through-the-this-pointer/4643295
    using BinaryTree<std::pair<K, V>>::_root;
    using BinaryTree<std::pair<K, V>>::left_rotate;

    BinarySearchTree() {}
    BinarySearchTree(const std::vector<std::optional<std::pair<K, V>>>& list) :
        BinaryTree<std::pair<K, V>>(std::move(list)) {}

    std::optional<V> search(const K& key, int* compare_count = nullptr) {
        Node* node = nullptr;
        int cmpcnt = 0;

        std::tie(node, cmpcnt) = find(key);

        if (compare_count) *compare_count = cmpcnt;

        if (!node) return {};

        return node->value->second;
    }

    void left_rotate(const K& key) {
        Node* node = nullptr;
        std::tie(node, std::ignore) = find(key);
        if (!node) return;
        if (node->right == nullptr) return;
        // 基类 left_roate
        left_rotate(node);
    }

    // TODO: 实现下面这些
    void right_rotate(const K& key) {
    }
    
    // 求最小
    std::optional<K> minimum() {
    }

    // 求最大
    std::optional<K> maximum() {
    }

    template <typename Key, typename Value>
    void insert(Key&& key, Value&& val) {
    }

    bool remove(const K& key) {
        return false;
    }
protected:
    // 返回查找到的节点，以及比较次数
    std::pair<Node*, int> find(const K& key) {
        if (!_root) return std::make_pair(nullptr, 0);

        int cmpcnt = 0;

        auto p = _root;
        while (p) {
            if (key == p->value->first) {
                ++cmpcnt;
                return std::make_pair(p, cmpcnt);
            } else if (key < p->value->first) {
                ++cmpcnt;
                p = p->left;
            } else {
                p = p->right;
            }
        }
        return std::make_pair(nullptr, cmpcnt);
    }
};

} // namespace dsa

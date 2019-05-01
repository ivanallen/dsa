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

#include "binary_tree.h"

namespace dsa {

template <typename K, typename V>
class BinarySearchTree : public BinaryTree<std::pair<K, V>> {
public:
    using Node = typename BinaryTree<std::pair<K, V>>::Node;
    
    // https://stackoverflow.com/questions/4643074/why-do-i-have-to-access-template-base-class-members-through-the-this-pointer/4643295
    using BinaryTree<std::pair<K, V>>::_root;
    using BinaryTree<std::pair<K, V>>::left_rotate;
    using BinaryTree<std::pair<K, V>>::right_rotate;

    using ValueType = std::pair<K, V>;

    BinarySearchTree() {}
    explicit BinarySearchTree(const std::vector<std::optional<std::pair<K, V>>>& list) :
        BinaryTree<std::pair<K, V>>(list) {}

    std::optional<V> search(const K& key, int* compare_count = nullptr) const {
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
        if (node->right) return;
        // 基类 left_roate
        left_rotate(node);
    }

    void right_rotate(const K& key) {
        Node* node = nullptr;
        std::tie(node, std::ignore) = find(key);
        if (!node) return;
        if (!node->left) return;
        right_rotate(node);
    }

    std::optional<std::pair<K, V>> successor(const K& key) const {
        Node* node = nullptr;
        std::tie(node, std::ignore) = find(key);
        // FIXME: Maybe we need throw exception here.
        if (!node) return std::nullopt;

        auto s = successor(node);
        if (!s) return std::nullopt;
        return std::make_pair(s->value->first, s->value->second);
    }

    std::optional<std::pair<K, V>> predecessor(const K& key) const {
        Node* node = nullptr;
        std::tie(node, std::ignore) = find(key);
        // FIXME: Maybe we need throw exception here.
        if (!node) return std::nullopt;

        auto s = predecessor(node);
        if (!s) return std::nullopt;
        return std::make_pair(s->value->first, s->value->second);
    }

    template <typename Key, typename Value>
    void insert(Key&& key, Value&& val) {
    }

    bool remove(const K& key) {
        return false;
    }
protected:
    // 返回查找到的节点，以及比较次数
    std::pair<Node*, int> find(const K& key) const {
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

    // 查询子树 u 中的最小节点
    Node* minimum(Node* u) const {
        assert(u != nullptr);
        while (u->left) {
            u = u->left;
        }
        return u;
    }

    // 查询子树 u 中的最大节点
    Node* maximum(Node* u) const {
        assert(u != nullptr);
        while (u->right) {
            u = u->right;
        }
        return u;
    }

    /*
      case 1: u 没有右孩子

            u
          /   \
         a     b

      case 2: u 有右孩子

            u.p.p <- we found u's successor
             /
           u.p
          /   \
         a     u
              /
             b
     */
    // 我们说 u 的后继，指经过中序遍历后，排在 u 后面的那个元素。
    Node* successor(Node* u) const {
        assert(u != nullptr);
        // case 1: u 有右孩子
        if (u->right) {
            return minimum(u->right);
        }
        // case 2: u 没有右孩子
        // 对于没有右孩子的 u，我们称 u 为子树 u.p 的右界(最大节点)。
        // 如果 u.p 仍然是 u.p.p 的右孩子，则 u 同样也为子树 u.p.p 的右界
        // 不断向上搜寻，直到 u 不是 u.p.p...p 的右界为止
        auto v = u->p;
        while (v && v->right == u) {
            u = v;
            v = u->p;
        }
        return v;
    }

    // 查找前驱，successor 的镜像算法
    Node* predecessor(Node* u) const {
        assert(u != nullptr);
        // case 1: u 有左孩子
        if (u->left) {
            return maximum(u->left);
        }
        // case 2: u 没有左孩子。u 是 u.p 的左界
        auto v = u->p;
        while (v && v->left == u) {
            u = v;
            v = u->p;
        }
        return v;
    }

    // TODO: 
    void remove(Node* u) {
        assert(u != nullptr);
    }
};

} // namespace dsa

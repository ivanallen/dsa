/*
 * Copyright (c) 2019, DSA authors.
 * All rights reserved.
 * Authors: Allen
 */

// 红黑树
#pragma once

#include <memory>
#include <queue>
#include <vector>
#include <optional>

#include "binary_search_tree.h"

namespace dsa {

template <typename K, typename V>
class RedBlackTree : public BinarySearchTree<K, V> {
public:
    using Node = typename BinarySearchTree<K, V>::Node;
    using BinarySearchTree<K, V>::_root;
    using BinarySearchTree<K, V>::left_rotate;
    using BinarySearchTree<K, V>::right_rotate;
    using BinarySearchTree<K, V>::create_node;

    RedBlackTree() {}

    // 插入键值对
    // 不允许插入重复 key
    template <typename Key, typename Value>
    bool insert(Key&& key, Value&& val) {
        auto z = create_node(std::make_pair(std::forward<Key>(key), std::forward<Value>(val)));
        z->color = ColorType::RED;

        if (!_root) {
            z->color = ColorType::BLACK;
            _root = z;
            return true;
        }

        // 查询应该插入的位置
        auto u = _root;
        auto v = u->p; // 总是指向 u 的父节点
        while (u) {
            v = u;
            if (key == u->value->first) {
                return false;
            } else if (key < u->value->first) {
                u = u->left;
            } else {
                u = u->right;
            }
        }

        // 到此 u 为空，v 表示叶子节点。z 应该插入 u 下面。
        if (key < v->value->first) {
            v->left = z;
        } else {
            v->right = z;
        }
        // 建立父节点关系
        z->p = v;

        // 此时 rbtree 可能性质被破坏，需要 fix
        insert_fixup(z);
        return true;
    }

protected:
    // fixup 算法其实就是 2-3 树的 fixup. 关于 2-3 树，我们在文档中会有详细的介绍。
    // 这是一个 bottom-to-up 的过程
    // 基本思想：
    // 1. 新节点为红节点
    // 2. 如果父节点是黑节点，什么也不做
    // 3. 如果父节点是红节点，判断父节点是 2-node 还是 3-node. 我们考虑父节点是左孩子的情况
    //   3.1 2-node: 父节点及叔节点着黑色
    //   3.2 3-node: 3-node 需要做旋转操作。判断当前新节点 z 在 3-node 的位置。
    //     3.2.1 左: right_rotate(z.p.p)，同时 z.p.p.color = RED, z.p.color = BLACK
    //     3.2.2 中：执行 left_rotate(z.p) 变成 3.2.1 中的情况
    //     3.2.3 右：不可能出现。因为 z.p 是左孩子
    void insert_fixup(Node* z) {
        while (z->p && z->p->color == ColorType::RED) {
            // 注意了，z->p->p 是一定存在的，因为 z->p->color 为红
            // 现在我们考虑 z 父节点是左孩子的情况
            if (z->p == z->p->p->left) {
                // 判断是 2-node 还是 3-node
                // 如果 z 的叔节点是红节点，说明是 2-node，否则是 3-node
                if (z->p->p->right && z->p->p->right->color == ColorType::RED) {
                    // 2-node 处理方法
                    z->p->color = ColorType::BLACK;
                    z->p->p->right->color = ColorType::BLACK;
                    z = z->p->p;
                    z->color = ColorType::RED; // 注意要维护黑高。上面多了一层黑高，这里减掉。
                } else {
                    // 3-node 处理方法
                    // 判断 z 在 3-node 中的位置
                    if (z == z->p->right) {
                        // 中间位置，左旋，转变成左侧位置
                        // 这样 3.2.2 就能和 3.2.1 统一处理了
                        z = z->p;
                        left_rotate(z);
                    }
                    z->p->p->color = ColorType::RED; // z.p.p 当前一定是黑色，因为 z.p 是红色。
                    z->p->color = ColorType::BLACK;
                    right_rotate(z->p->p);
                }
            } else {
                // 镜像算法。
                if (z->p->p->left && z->p->p->left->color == ColorType::RED) {
                    // 2-node 处理方法
                    z->p->color = ColorType::BLACK;
                    z->p->p->left->color = ColorType::BLACK;
                    z = z->p->p;
                    z->color = ColorType::RED; // 注意要维护黑高。上面多了一层黑高，这里减掉。
                } else {
                    // 3-node 处理方法
                    if (z == z->p->left) {
                        z = z->p;
                        right_rotate(z);
                    }
                    z->p->p->color = ColorType::RED; // z.p.p 当前一定是黑色，因为 z.p 是红色。
                    z->p->color = ColorType::BLACK;
                    left_rotate(z->p->p);
                }
            }
        }
        _root->color = ColorType::BLACK;
    }
};

} // namespace dsa

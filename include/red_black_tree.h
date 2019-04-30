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
#include <iostream>

#include "binary_search_tree.h"

namespace dsa {

template <typename K, typename V>
class RedBlackTree : public BinarySearchTree<K, V> {
public:
    using Node = typename BinarySearchTree<K, V>::Node;
    using BinarySearchTree<K, V>::_root;
    using BinarySearchTree<K, V>::find;
    using BinarySearchTree<K, V>::left_rotate;
    using BinarySearchTree<K, V>::right_rotate;
    using BinarySearchTree<K, V>::create_node;
    using BinarySearchTree<K, V>::release_node;
    using BinarySearchTree<K, V>::transplant;
    using BinarySearchTree<K, V>::successor;
    using BinarySearchTree<K, V>::predecessor;

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

    bool remove(const K& key) {
        Node* u = nullptr;
        std::tie(u, std::ignore) = find(key);
        if (!u) return false;
        remove(u);
        return true;
    }

protected:
    // fixup 算法其实就是 2-3-4 树的 fixup. 关于 2-3-4 树，我们在文档中会有详细的介绍。
    // 这是一个 bottom-to-up 的过程
    // 基本思想：
    // 1. 新节点为红节点
    // 2. 如果父节点是黑节点，什么也不做
    // 3. 如果父节点是红节点，判断父节点是 4-node 还是 3-node. 我们考虑父节点是左孩子的情况
    //   3.1 4-node: 父节点及叔节点着黑色
    //   3.2 3-node: 3-node 需要做旋转操作。判断当前新节点 z 在 3-node 的位置。
    //     3.2.1 左: right_rotate(z.p.p)，同时 z.p.p.color = RED, z.p.color = BLACK
    //     3.2.2 中：执行 left_rotate(z.p) 变成 3.2.1 中的情况
    //     3.2.3 右：不可能出现。因为 z.p 是左孩子
    void insert_fixup(Node* z) {
        while (z->p && z->p->color == ColorType::RED) {
            // 注意了，z->p->p 是一定存在的，因为 z->p->color 为红
            // 现在我们考虑 z 父节点是左孩子的情况
            if (z->p == z->p->p->left) {
                // 判断是 4-node 还是 3-node
                // 如果 z 的叔节点是红节点，说明是 4-node，否则是 3-node
                if (z->p->p->right && z->p->p->right->color == ColorType::RED) {
                    // 4-node 处理方法
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
                    // 4-node 处理方法
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

    // rb-tree 的 remove 也是一个 2-3-4 树的 remove 过程
    // 首先，按照正常 bst 的删除方法，删除 u，但是先得记住 u 的颜色
    void remove(Node* u) {
        assert(u != nullptr);
        auto star_color = u->color;
        Node* x = nullptr; // star 节点（打星号标记的节点）
        Node* xp = nullptr; // x 的爸爸
        if (!u->left) {
            x = u->right;
            // u 即将被删除，因此未来 x.p 就是 u.p
            xp = u->p;
            transplant(u, u->right);
        } else if (!u->right) {
            x = u->right;
            xp = u->p;
            transplant(u, u->left);
        } else {
            // 找到 u 的后继(一定存在)
            // 或者前驱也行
            // auto v = successor(u);
            // 为了验证答案，这里采用前驱替代的办法。
            // 这个工具可以在网页上观察 rb-tree 插入删除过程 https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
            auto v = predecessor(u);
            star_color = v->color; // v 需要占居 u 的位置
            x = v->right;
            xp = v->p;
            transplant(v, v->right); // v.right 替换 v, v 游离
            transplant(u, v); // v 替换掉 u
            v->left = u->left;
            v->right = u->right;
            v->color = u->color;
        }

        release_node(u);

        // star 节点具有两个颜色，x->color + star_color
        // 如果 star color 是红，直接返回就行，因为不会破坏 rbtree 的性质
        if (star_color == ColorType::RED) return;
        // star 节点始终用来追踪子树负债情况，如果 star 节点最开始是黑色的
        // 表示 star 到 root 这条路径上欠了一个黑节点，这是需要补偿的
        // 针对 star 节点开始做修复
        remove_fixup(x, xp);
    }

    // fixup 流程其实也是 2-3-4-tree 的 fixup 流程
    // 我们只讨论删除叶子节点的情况。因为所有删除操作都可以化归到删除叶子节点。
    // 先来看看2-3-4-tree 删除操作的基本思想：
    //      如果叶子节点是 3 节点或者 4 节点，很好办，直接删除就行了。不需要 fix
    //      如果叶子节点是 2 节点，这就有点麻烦，它需要跪求兄弟节点借一个节点给它。
    //      我们考虑兄弟节点是否是个有钱的主（兄弟是 3 节点或 4 节点)。
    //      如果兄弟有钱，一切好说，从兄弟拿一个值到父节点，同时父节点再拿一个值补偿给缺失的叶子
    //      如果兄弟节点没钱，没办法，那只能找爸爸借了。同样的爸爸要是没钱也得找叔叔借，叔叔没钱找爷爷。
    //
    // 下面我们来看一下 rbtree 中的情况
    //      关于涂色：
    //      把一个节点涂红，意味着做了一个 merge 操作，相当于 2-3-4-tree 中把当前节点，上推合入到父节点中
    //      把一个节点涂黑，意味着做了一个分裂操作，即将节点下推分裂出去。
    //
    // 来看一下 rbtree 删除是如何 fix 的。我们先讨论最简单的情况
    //
    // case 1:
    //
    // 我们用字母加 ！表示一个节点是红色节点
    // 用字母加 * 表示这是一个 star 节点
    // 用字母加 # 表示这个节点颜色我们并不关心
    // 只有一个字母则表示这是一个黑色节点
    //
    //                A#
    //              /   \
    //             B*    C
    //                 /   \
    //                D#    E!
    //
    // 上面这种情况，B* 子树表示缺失一个黑色节点，B* 要求能从兄弟 C 借一个节点过来。很开心，兄弟 C 是富裕的，
    // 因为兄弟 C 至少是一个 3-node 节点（因为 E! 是红色的）
    // 借的方式非常简单，我们先把节点 C 和父节点 A 合并（上推），再把 A 分裂出来(下推)，这样 B 所在的路径的缺失的黑色就被 A 补偿了
    // 补偿动作结束！
    //
    //                C#
    //              /    \
    //             A      E
    //           /   \
    //          B     D#
    //
    //  其实上面的变换非常简单，对 A 做一个左旋操作，同时 C.color = A.color, A.color = BLACK, E.color = BLACK 即可
    //  说形象点，就是 C 错给了 A, A 再借给了 B
    //
    //  case 2:
    //  这种情况可能就没有上面的容易变换，比如
    //
    //                A#
    //              /   \
    //             B*    C
    //                 /   \
    //                D!    E
    //
    // 虽然 C 是富裕的，但是 C 喜欢把钱藏兜里，因为右孩子是黑色的，而左孩子 D 才是红色的，
    // 对于这种情况，想借出 C 和 D! 没那么容易。
    // 直接左旋 A 不行，因为D! 并不能成功补偿 B*。我们需要对 C 做一次右旋操作，把 case 2 变成 case1
    // 这样 B 就能快乐的找兄弟借了。
    //
    //                A#
    //              /   \
    //             B*    D
    //                 /   \
    //                      C!
    //                    /   \
    //                         E
    //
    //  case 3:
    //  有时候，兄弟也很穷啊！再怎么变还是穷。比如下面这样
    //
    //                A#
    //              /   \
    //             B*    C
    //                 /   \
    //                D     E
    //
    // 这时候怎么办呢，既然兄弟穷，那就找爸爸。爸爸为了保证公平性，得把兄弟 C 先涂红，相当于兄弟 C 也合入到 A 了
    // A 可能是红节点，也可能是黑节点，如果 A 是红的，说明老爸很有钱，直接把 A 变成黑，分裂出来就能直接补偿了
    // 如果老爸 A 已经是黑的，说明爸爸也穷，那爸爸怎么办，只好他的兄弟借了。于是，再得进行一轮循环操作了
    //
    //                A#*
    //              /    \
    //             B      C!
    //                  /   \
    //                 D     E
    //
    //  变换完成后，如果 A 是红的，直接改成黑色，补偿就结束了。如果 A 是黑的，接着找 A 的兄弟借吧。
    //
    //  case 4:
    //  其实吧，最怕的就是连兄弟的都没有的人，就比如下面的情况。
    //  在 rbtree 中 B 的兄弟是 C，C 是红节点，但是在 2-3-4-tree 中其实是 C 是 A 节点的姐妹节点
    //
    //                A
    //              /   \
    //             B*    C!
    //                 /   \
    //                D     E
    //
    // 这很好办，没有兄弟，创造兄弟，把 D 让给你 B* 了，对 C 做一个左旋操作。
    //
    //                C
    //              /   \
    //             A!    E
    //           /  \
    //          B*   D
    //
    void remove_fixup(Node* x, Node* xp) {
        while (xp && (!x || x->color == ColorType::BLACK)) {
            if (!x || x == xp->left) { // x && x== xp->left if using successor
                auto w = xp->right;
                if (w && w->color == ColorType::RED) { // case 4: 没兄弟，制造兄弟
                    w->color = ColorType::BLACK;
                    xp->color = ColorType::RED;
                    left_rotate(xp);
                    // 转向 case 1/2/3
                    w = xp->right;
                }

                // 到这里，w 一定是黑色的
                if (!w || (black(w->left) && black(w->right))) { // case 3: 兄弟也穷
                    if (w) w->color = ColorType::RED;
                    x = xp;
                    xp = x->p;
                } else {
                    // 这里 w 一定不空，因为兄弟有钱啊
                    if (black(w->right)) { // case 2: 兄弟有钱，但是藏起来了
                        w->left->color = ColorType::BLACK;
                        w->color = ColorType::RED;
                        right_rotate(w);
                        w = xp->right;
                    }

                    // case 1: 兄弟有钱，也愿意贡献
                    w->right->color = ColorType::BLACK;
                    w->color = xp->color;
                    xp->color = ColorType::BLACK;
                    left_rotate(xp);
                    break;
                }
            } else {
                // 镜像算法
                auto w = xp->left;
                if (w && w->color == ColorType::RED) { // case 4: 没兄弟，制造兄弟
                    w->color = ColorType::BLACK;
                    xp->color = ColorType::RED;
                    right_rotate(xp);
                    // 转向 case 1/2/3
                    w = xp->left;
                }

                // 到这里，w 一定是黑色的
                if (!w || (black(w->left) && black(w->right))) { // case 3: 兄弟也穷
                    if (w) w->color = ColorType::RED;
                    x = xp;
                    xp = x->p;
                } else {
                    // 这里 w 一定不空
                    if (black(w->left)) { // case 2: 兄弟有钱，但是藏起来了
                        w->right->color = ColorType::BLACK;
                        w->color = ColorType::RED;
                        left_rotate(w);
                        w = xp->left;
                    }

                    // case 1: 兄弟有钱，也愿意贡献
                    w->left->color = ColorType::BLACK;
                    w->color = xp->color;
                    xp->color = ColorType::BLACK;
                    right_rotate(xp);
                    x = _root;
                    break;
                }
            }
        }

        if (x) x->color = ColorType::BLACK;
    }

    bool black(Node* u) const {
        return !u || u->color == ColorType::BLACK;
    }
};

} // namespace dsa

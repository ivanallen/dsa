/*
 * Copyright (c) 2019, ivan_allen@163.com 
 * All rights reserved.
 */

// 二叉树基类
// 所有二叉树继承此基类
// 另一个 binary_tree.h 要删除掉，实现的复杂且不合理

#pragma once

#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <vector>
#include <optional>
#include <iostream>
#include <sstream>
#include <cassert>

namespace dsa {

// TODO: 抽取类 class Color
enum ColorType {
    NONE = 0,
    WHITE = 1,
    RED = 2,
    BLACK = 3,
};
// 返回颜色值
std::string color(ColorType c) {
    switch (c) {
    case ColorType::NONE:
    case ColorType::WHITE:
        return "\x1b[0m";
    case ColorType::RED:
        return "\x1b[31m";
    case ColorType::BLACK:
        return "\x1b[0m";
    }
}

// 二叉树
template <typename T>
class BinaryTree {
protected:
    // Node 作为 BinaryTree 的内部类，不暴露给外部
    struct Node;
public:
    BinaryTree() {}

    // 从数组构建二叉树
    BinaryTree(const std::vector<std::optional<T>>& list) {
        std::vector<Node*> nodes;
        for (auto& node : list) {
            if (node) {
                nodes.push_back(new Node(*node));
            } else {
                nodes.push_back(nullptr);
            }
        }
        init(nodes);
    }

    virtual ~BinaryTree() {
        // 不能递归删除，可能会爆栈
        if (!_root) return;
        std::queue<Node*> todo;

        todo.push(_root);

        while (!todo.empty()) {
            auto node = todo.front();
            todo.pop();
            if (node->left) todo.push(node->left);
            if (node->right) todo.push(node->right);
            delete node;
        }
        _root = nullptr;
    }
    
    // 对外只暴露值
    void preorder(const std::function<void (const T&)>& cb) const {
        preorder([&cb](Node* node) {
            cb(*node->value);
        });
    }

    void inorder(const std::function<void (const T&)>& cb) const {
        inorder([&cb](Node* node) {
            cb(*node->value);
        });
    }

    // 直接根据值进行旋转。
    // 如果有重复值，优先旋转深度最小最靠左的那个。
    // FIXME: v 如果没有实现和 ostream 的对接，这里 oss 会编译失败。
    void left_rotate(const T& v) {
        auto node = find(v);
        if (!node) {
            std::stringstream oss;
            oss << "cannot find node `" << v <<  "`";
            throw std::runtime_error(oss.str());
        }

        if (node->right == nullptr) {
            std::stringstream oss;
            oss << "cannot left rotate at node `" << v <<  "`";
            throw std::runtime_error(oss.str());
        }
        left_rotate(node);
    }

    // FIXME: v 如果没有实现和 ostream 的对接，这里 oss 会编译失败。
    void right_rotate(const T& v) {
        auto node = find(v);
        if (!node) {
            std::stringstream oss;
            oss << "cannot find node `" << v <<  "`";
            throw std::runtime_error(oss.str());
        }
        if (node->left== nullptr) {
            std::stringstream oss;
            oss << "cannot right rotate at node `" << v <<  "`";
            throw std::runtime_error(oss.str());
        }
        right_rotate(node);
    }

    // TODO
    std::optional<T> predecessor(const T& v) {
        auto node = find(v);
        return {};
    }

    // TODO
    std::optional<T> successor(const T& v) {
        auto node = find(v);
        return {};
    }

    // 生成前序和中序序列返回
    // TODO: 或许可以做成可视化？
    std::string dump() const {
        std::stringstream oss;
        oss << "preorder:[";
        bool first = true;
        preorder([&oss, &first](Node* node) {
            if (first) {
                oss << color(node->color);
                oss << *node->value;
                oss << color(ColorType::NONE);
                first = false;
                return;
            }
            oss << "," << color(node->color);
            oss << *node->value;
            oss << color(ColorType::NONE);
        });
        oss << "]" << std::endl;

        oss << "inorder: [";
        first = true;
        inorder([&oss, &first](Node* node) {
            if (first) {
                oss << color(node->color);
                oss << *node->value;
                oss << color(ColorType::NONE);
                first = false;
                return;
            }
            oss << "," << color(node->color);
            oss << *node->value;
            oss << color(ColorType::NONE);
        });
        oss << "]";
        return oss.str();
    }
protected:
    // 从数组构建二叉树
    // 这个函数可以很方便的自动构建二叉树，适合自动化测试
    // 数组保存的是一个完全二叉树（包含空节点）
    // 因此可以使用基于队列的算法进行构建
    void init(const std::vector<Node*> nodes) {
        std::queue<Node*> todo;
        auto it = nodes.begin();
        if (it == nodes.end()) return;

        _root = *it++;
        todo.push(_root);

        for (;;) {
            auto front = todo.front();
            todo.pop();

            // 找到左孩子，挂接到 front 下
            auto left = it++;
            if (left == nodes.end()) break;
            if (front != nullptr) {
                front->left = *left;
                todo.push(front->left);
                if (front->left) front->left->p = front; // 构建到父节点的指针
            } else {
                todo.push(nullptr);
            }

            // 找到右孩子，挂接到 front 下
            auto right = it++;
            if (right == nodes.end()) break;
            if (front != nullptr) {
                front->right = *right;
                todo.push(front->right);
                if (front->right) front->right->p = front;
            } else {
                todo.push(nullptr);
            }
        }
    }

    // void cb(Node*)
    // 迭代版本中序遍历
    // 栈后访问法
    template <typename FUNC>
    void preorder(FUNC&& cb) const {
        if (!_root) return;
        std::stack<Node*> todo;

        todo.push(_root);
        while (!todo.empty()) {
            auto node = todo.top();
            todo.pop();

            cb(node);

            if (node->right) todo.push(node->right);
            if (node->left) todo.push(node->left);
        }
    }

    template <typename FUNC>
    void inorder(FUNC&& cb) const {
        if (!_root) return;
        std::stack<Node*> todo;

        auto p = _root;
        while (p) {
            todo.push(p);
            p = p->left;
        }
        while (!todo.empty()) {
            auto node = todo.top();
            todo.pop();

            cb(node);

            p = node->right;
            while (p) {
                todo.push(p);
                p = p->left;
            }
        }
    }

    /*
            u_p
             |
             u
           /   \
          a     c

             ||

             u_p

              u
            /   \
           a     c
     */
    // 将子树 u 从原树中摘下
    // 返回 u 的父节点
    // 注意 detach 操作一定要干净，即脱离后，不应该与原树有任何指针来往
    Node* detach(Node* u, bool* left = nullptr) {
        assert(u != nullptr);
        auto u_p = u->p;
        if (!u_p) return nullptr;

        u->p = nullptr;
        if (u_p->left == u) {
            if (left) *left = true;
            u_p->left = nullptr;
        } else {
            if (left) *left = false;
            u_p->right = nullptr;
        }
        return u_p;
    }

    // 使用 v 替换 u
    // 即把 u/v 从树中摘下，然后把 v 放到原来 u 的位置
    // FIXME: 有些 transplant 是无法成功的。比如：v 是 u 的祖先
    void transplant(Node* u, Node* v) {
        // 禁止
        assert(u != nullptr);
        assert(u != v);

        bool left = false;
        auto u_p = detach(u, &left);
        detach(v);

        if (u_p) {
            if (left) {
                u_p->left = v;
            } else {
                u_p->right = v;
            }
        } else {
            // 说明 u 是 root
            _root = v;
        }

        if (v) v->p = u_p;
    }

    /*
            |
            x
          /   \
         a     y
             /   \
            b     c

               || 

                |
                y
              /   \
             x     c
           /   \
          a     b

     */
    // 对 x 左旋
    void left_rotate(Node* x) {
        assert(x != nullptr);
        assert(x->right != nullptr);

        auto y = x->right;
        transplant(x, y);

        auto b = y->left;

        if (b) {
            detach(b);
            x->right = b;
            b->p = x;
        }
        y->left = x;
        x->p = y;
    }

    void right_rotate(Node* y) {
        assert(y != nullptr);
        assert(y->left != nullptr);

        auto x = y->left;
        transplant(y, x);

        auto b = x->right;
        if (b) {
            detach(b);
            y->left = b;
            b->p = y;
        }

        x->right = y;
        y->p = x;
    }

    // 普通的二叉树只能顺序查找
    // 子类可以重写此方法，以完成高效查找
    virtual Node* find(const T& v) {
        if (!_root) return nullptr;
        std::queue<Node*> todo;

        todo.push(_root);

        while (!todo.empty()) {
            auto node = todo.front();
            todo.pop();
            if (*node->value == v) {
                return node;
            }
            if (node->left) todo.push(node->left);
            if (node->right) todo.push(node->right);
        }
        return nullptr;
    }

    // TODO:
    Node* successor(Node* u) {
        return nullptr;
    }

    Node* predecessor(Node* u) {
        return nullptr;
    }

protected:
    Node* _root = nullptr;
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const BinaryTree<T>& tree) {
    return  out << tree.dump();
}

// 内部节点
template <typename T>
struct BinaryTree<T>::Node {
    Node* left = nullptr;
    Node* right = nullptr;
    Node* p = nullptr;
    ColorType color = ColorType::NONE;
    std::unique_ptr<T> value;

    template <typename Value>
    Node(Value&& v) {
        value.reset(new T(std::forward<Value>(v)));
    }
};

} // namespace dsa

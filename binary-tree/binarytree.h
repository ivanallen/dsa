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

    // 生成前序和中序序列返回
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
private:
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

    // void cb(K&, V&)
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

private:
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

#pragma once

#include <memory>
#include <queue>
#include <vector>
#include <optional>
#include <iostream>

template <typename K, typename V>
struct Node {
    Node(const K& key, const V& value) {
        this->key = key;
        this->value = value;
    }

    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    std::unique_ptr<Node> p;
    K key;
    V value;
};

template <typename K, typename V>
class BinaryTree {
public:
    using TreeNode = Node<K, V>;
    BinaryTree(std::vector<std::unique_ptr<TreeNode>>&& list) {
        _init(std::move(list));
    }

    BinaryTree(std::vector<std::optional<std::pair<K, V>>>&& list) {
        std::vector<std::unique_ptr<TreeNode>> l;
        for (auto& node : list) {
            if (node) {
                l.emplace_back(new TreeNode(node->first, node->second));
            } else {
                l.emplace_back(nullptr);
            }
        }
        _init(std::move(l));
    }

    // 中序
    void inorder(const std::function<void (const K&, const V&)>& cb) {
        _inorder(_root.get(), cb);
    }

private:
    void _init(std::vector<std::unique_ptr<TreeNode>>&& list) {
        std::queue<TreeNode*> _q;
        auto it = list.begin();
        if (it == list.end()) return;

        _root.swap(*it);
        _q.push(_root.get());

        ++it;
        for (;;) {
            TreeNode* front = _q.front();
            _q.pop();

            auto left = it++;
            if (left == list.end()) break;
            if (front != nullptr) {
                front->left.swap(*left);
                _q.push(front->left.get());
            } else {
                _q.push(nullptr);
            }

            auto right = it++;
            if (right == list.end()) break;
            if (front != nullptr) {
                front->right.swap(*right);
                _q.push(front->right.get());
            } else {
                _q.push(nullptr);
            }
        }
    }

    void _inorder(TreeNode* x, const std::function<void (const K&, const V&)>& cb) {
        if (x == nullptr) return;

        _inorder(x->left.get(), cb);
        cb(x->key, x->value);
        _inorder(x->right.get(), cb);
    }

protected:
    TreeNode* root() {
        return _root.get();
    }

private:
    std::unique_ptr<TreeNode> _root;
};

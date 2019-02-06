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
        this->p = nullptr;
    }

    void set_left(Node* node) {
        if (node == nullptr) return;
        left.reset(node);
        node->p = this;
    }

    void set_right(Node* node) {
        if (node == nullptr) return;
        right.reset(node);
        node->p = this;
    }

    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    Node* p;
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
    void inorder(const std::function<void (const K&, const V&)>& cb) const {
        _inorder(_root.get(), cb);
    }

    // 前序
    void preorder(const std::function<void (const K&, const V&)>& cb) const {
        _preorder(_root.get(), cb);
    }

    void postorder(const std::function<void (const K&, const V&)>& cb) const {
        _postorder(_root.get(), cb);
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
            if (!is_null(front)) {
                front->left.swap(*left);
                _q.push(front->left.get());

                if (!is_null(front->left.get())) {
                    front->left->p = front;
                }
            } else {
                _q.push(nullptr);
            }

            auto right = it++;
            if (right == list.end()) break;
            if (!is_null(front)) {
                front->right.swap(*right);
                _q.push(front->right.get());

                if (!is_null(front->right.get())) {
                    front->right->p = front;
                }
            } else {
                _q.push(nullptr);
            }

        }
    }

    void _inorder(TreeNode* x, const std::function<void (const K&, const V&)>& cb) const {
        if (is_null(x)) return;

        _inorder(x->left.get(), cb);
        cb(x->key, x->value);
        _inorder(x->right.get(), cb);
    }

    void _preorder(TreeNode* x, const std::function<void (const K&, const V&)>& cb) const {
        if (is_null(x)) return;

        cb(x->key, x->value);
        _preorder(x->left.get(), cb);
        _preorder(x->right.get(), cb);
    }

    void _postorder(TreeNode* x, const std::function<void (const K&, const V&)>& cb) const {
        if (is_null(x)) return;

        _postorder(x->left.get(), cb);
        _postorder(x->right.get(), cb);
        cb(x->key, x->value);
    }
protected:
    TreeNode* root() {
        return _root.get();
    }

    void set_root(TreeNode* x) {
        _root.reset(x);
    }

    bool is_left_child(TreeNode* x) {
        if (is_null(x->p)) {
            return true;
        }

        if (x->p->left.get() == x) {
            return true;
        }
        return false;
    }

    // 将 x 节点从树中摘除，但是不销毁
    // 同时返回 x 以前的父节点
    TreeNode* detach(TreeNode* x) {
        if (is_null(x)) {
            return nullptr;
        }

        if (is_null(x->p)) {
            _root.release();
            return nullptr;
        }

        if (x->p->left.get() == x) {
            x->p->left.release();
        } else {
            x->p->right.release();
        }
        auto p = x->p;
        x->p = nullptr;
        return p;
    }

    // 使用子树 v 替换掉子树 u
    // 替换后，子树 u 自动从树上被摘下，交返回 u
    // 注意！使用此函数前，请先将 v 从树中摘除
    TreeNode* transplant(TreeNode* u, TreeNode* v) {
        auto is_left = is_left_child(u);
        // 将 u 从树中摘除
        auto u_p = detach(u);

        if (is_null(u_p)) {
            set_root(v);
            return u;
        }

        if (is_left) {
            u_p->left.reset(v);
        } else {
            u_p->right.reset(v);
        }
        if (!is_null(v)) {
            v->p = u_p;
        }
        return u;
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
    bool left_rotate(TreeNode* x) {
        if (is_null(x)) return false;
        auto y = x->right.get();
        if (is_null(y)) return false;
        // 1. detach y
        detach(y);
        // 2. x->right = b
        auto b = y->left.get();
        detach(b);
        x->set_right(b);
        // 3. transplant(x, y)
        transplant(x, y);
        // 4. y->left = x
        y->set_left(x);
        return true;
    }

    bool right_rotate(TreeNode* y) {
        if (is_null(y)) return false;
        auto x = y->left.get();
        if (is_null(x)) return false;
        // 1. detach y
        detach(x);
        // 2. y->left = b
        auto b = x->right.get();
        detach(b);
        y->set_left(b);
        // 3. transplant(y, x)
        transplant(y, x);
        // 4. x->right = y
        x->set_right(y);
        return true;
    }

    virtual bool is_null(TreeNode* x) const {
        return x == nullptr;
    }
private:
    std::unique_ptr<TreeNode> _root;
};

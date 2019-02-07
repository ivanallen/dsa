#pragma once

#include <memory>
#include <queue>
#include <vector>
#include <optional>
#include <iostream>

enum class Color {
    RED = 0,
    BLACK = 1,
};

template <typename K, typename V>
struct Node {
    Node() {
        this->p = nullptr;
        this->key = K();
        this->key = V();
    }

    Node(const K& key, const V& value) {
        this->key = key;
        this->value = value;
        this->p = nullptr;
    }

    ~Node() {
#ifdef _DEBUG
        std::cout << "~Node(): address:" << this << " key=" << key << std::endl;
#endif
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
    Color color;
    K key;
    V value;
};


template <typename K, typename V>
class BinaryTree {
public:
    using TreeNode = Node<K, V>;

    BinaryTree() {}

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
    void inorder(const std::function<void (const K&, const V&, Color)>& cb) const {
        _inorder(_root.get(), cb);
    }

    // 前序
    void preorder(const std::function<void (const K&, const V&, Color)>& cb) const {
        _preorder(_root.get(), cb);
    }

    void postorder(const std::function<void (const K&, const V&, Color)>& cb) const {
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
            if (front != nullptr) {
                front->left.swap(*left);
                _q.push(front->left.get());

                if (front->left.get() != nullptr) {
                    front->left->p = front;
                }
            } else {
                _q.push(nullptr);
            }

            auto right = it++;
            if (right == list.end()) break;
            if (front != nullptr) {
                front->right.swap(*right);
                _q.push(front->right.get());

                if (front->right.get() != nullptr) {
                    front->right->p = front;
                }
            } else {
                _q.push(nullptr);
            }

        }
    }

    void _inorder(TreeNode* x, const std::function<void (const K&, const V&, Color)>& cb) const {
        if (is_leaf(x)) return;

        _inorder(x->left.get(), cb);
        cb(x->key, x->value, x->color);
        _inorder(x->right.get(), cb);
    }

    void _preorder(TreeNode* x, const std::function<void (const K&, const V&, Color)>& cb) const {
        if (is_leaf(x)) return;

        cb(x->key, x->value, x->color);
        _preorder(x->left.get(), cb);
        _preorder(x->right.get(), cb);
    }

    void _postorder(TreeNode* x, const std::function<void (const K&, const V&, Color)>& cb) const {
        if (is_leaf(x)) return;

        _postorder(x->left.get(), cb);
        _postorder(x->right.get(), cb);
        cb(x->key, x->value, x->color);
    }
protected:
    TreeNode* root() {
        return _root.get();
    }

    void set_root(TreeNode* x) {
        _root.reset(x);
    }

    bool is_root(TreeNode* x) {
        return x == _root.get();
    }

    bool is_left_child(TreeNode* x) {
        if (is_root(x)) {
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
        if (x == nullptr) {
            return nullptr;
        }

        if (is_root(x)) {
            _root.release();
            return x == nullptr ? nullptr : x->p;
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
#ifdef _DEBUG
        std::cout << "transplant u:" << u << "," << (u?u->key:-1)
            << " v:" << v << "," << (v?v->key:-1) << std::endl;
#endif
        auto is_left = is_left_child(u);

        if (is_root(u)) {
            // 将 u 从树中摘除
            auto u_p = detach(u);
#ifdef _DEBUG
            std::cout << "u_p:" << u_p << " transplant set root" << std::endl;
#endif
            set_root(v);
            if (v != nullptr) {
                v->p = u_p;
            }
            return u;
        }

        auto u_p = detach(u);
        if (is_left) {
            u_p->left.reset(v);
        } else {
            u_p->right.reset(v);
        }
        if (v != nullptr) {
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
#ifdef _DEBUG
        std::cout << "left rotate" << std::endl;
#endif
        if (x == nullptr) return false;
        auto y = x->right.get();
        if (y == nullptr) return false;
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
#ifdef _DEBUG
        std::cout << "right rotate:" << y << std::endl;
#endif
        if (y == nullptr) return false;
        auto x = y->left.get();
        if (x == nullptr) return false;
        // 1. detach x
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

    virtual bool is_leaf(TreeNode* x) const {
        return x == nullptr;
    }
private:
    std::unique_ptr<TreeNode> _root;
};

#pragma once

#include <binary-search-tree/binary_search_tree.h>

template <typename K, typename V>
class RbTree : public BinarySearchTree<K, V> {
public:
    using typename BinaryTree<K, V>::TreeNode;

    RbTree() {
        _guard = _make_null();
        //std::cout << "guard:" << _guard.get() << std::endl;
    }

    void insert(const K& key, const V& value) {
        auto z = _make_node(key, value);
        _insert(z.release());
    }

protected:
    bool is_leaf(TreeNode* x) const override {
        //std::cout << "is_leaf" << std::endl;
        return x == nullptr || x->left == nullptr || x->right == nullptr; 
    }

private:
    std::unique_ptr<TreeNode> _make_node(const K& key, const V& value) {
        auto z = std::make_unique<TreeNode>(key, value);
        z->color = Color::RED;
        z->left = _make_null();
        z->right = _make_null();
        z->left->p = z.get();
        z->right->p = z.get();
        z->p = _guard.get();

        return std::move(z);
    }

    // 所有叶子节点，是哨兵节点
    std::unique_ptr<TreeNode> _make_null() {
        auto nil = std::make_unique<TreeNode>();
        nil->color = Color::BLACK;
        return std::move(nil);
    }

    void _insert(TreeNode* z) {
        BinarySearchTree<K, V>::_insert(z);
        _insert_fixup(z);
    }

    void _insert_fixup(TreeNode* z) {
        while(z->p->color == Color::RED) {
            // 找叔节点
            if (this->is_left_child(z->p)) {
                auto y = z->p->p->right.get(); 

                if (y->color == Color::RED) {
                    y->color = Color::BLACK;                    // case 1
                    z->p->color = Color::BLACK;                 // case 1
                    z->p->p->color = Color::RED;                // case 1
                    z = z->p->p;                                // case 1
                } else {
                    if (!this->is_left_child(z)) {              // case 2
                        // 通过左旋，case 2 可以转换为 case 3
                        BinaryTree<K, V>::left_rotate(z->p);    // case 2
                    }                                           // case 2

                    z->p->color = Color::BLACK;                 // case 3
                    z->p->p->color = Color::RED;                // case 3
                    BinaryTree<K, V>::right_rotate(z->p->p);    // case 3
                }
            } else {
                // 和上面的 if 镜像
                auto y = z->p->p->left.get(); 
                if (y->color == Color::RED) {
                    y->color = Color::BLACK;
                    z->p->color = Color::BLACK;
                    z->p->p->color = Color::RED;
                    z = z->p->p;
                } else {
                    if (this->is_left_child(z)) {
                        BinaryTree<K, V>::right_rotate(z->p);
                    }
                    z->p->color = Color::BLACK;
                    z->p->p->color = Color::RED;
                    BinaryTree<K, V>::left_rotate(z->p->p);
                }
            }
        }
        this->root()->color = Color::BLACK;
    }

private:
    std::unique_ptr<TreeNode> _guard; // root 的父节点。
};

#pragma once
#include <queue>
#include <iostream>
#include <sstream>
#include <string>

template <typename Key, typename Value, size_t N=8>
class BTree {
public:
    BTree() {}
    ~BTree() {
        std::queue<Node*> q;
        if (!_root) {
            return;
        }

        q.push(_root);
        while (!q.empty()) {
            auto node = q.front();
            q.pop();
            for (int i = 0; i < N + 1; ++i) {
                if (node->children[i]) q.push(node->children[i]);
            }
            delete node;
        }
    }

    struct Node {
        Key keys[N]; // 浪费一个空间，用来做 guard
        std::unique_ptr<Value> vals[N];
        Node* children[N+1] = { nullptr };
        Node* parent = nullptr;
        int key_num = 0;
        std::string dump() {
            std::stringstream oss;
            if (key_num == 0) {
                oss << "[]";
                return oss.str();
            }
            oss << '[' << keys[0];
            for (int i = 1; i < key_num; ++i) {
                oss << "," << keys[i]; 
            }
            oss << ']';
            return oss.str();
        }
    };

    template <typename K, typename V>
    void insert(K&& key, V&& val) {
        _insert_into_leaf(std::forward<K>(key), std::forward<V>(val));
    }

    Value* find(const Key& key) {
        if (!_root) return nullptr;
        auto node = _root;
        while (node) {
            int index = _find_index(node, key);
            if (index != N && node->keys[index] == key) {
                return node->vals[index].get();
            }
            node = node->children[index];
        }
        return nullptr;
    }

    // TODO:
    bool remove(const Key& key) {
        return false;
    }

    void print() {
        std::queue<Node*> q;
        if (!_root) {
            std::cout << "[]" << std::endl;
            return;
        }

        q.push(_root);
        while (!q.empty()) {
            int size = q.size();
            while (size--) {
                auto node = q.front();
                q.pop();
                for (int i = 0; i < node->key_num + 1; ++i) {
                    if (node->children[i]) q.push(node->children[i]);
                }
                std::cout << node->dump() << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    template <typename K, typename V>
    void _insert_into_leaf(K&& key, V&& val) {
        auto leaf = _find_leaf(key);
        if (!leaf) {
            leaf = new Node;
            _root = leaf;
        }

        int index = _find_index(leaf, key);
        /*
        if (leaf->keys[index] == key) {
            leaf->vals[index].reset(new V(std::forward<V>(val)));
            return;
        }
        */
        // std::cout << "leaf:" << leaf << ":" << leaf->dump() << " insert:" << key << std::endl;

        // 直接插入
        for (int i = leaf->key_num - 1; i >= index; --i) {
            leaf->keys[i+1] = leaf->keys[i];
            leaf->vals[i+1] = std::move(leaf->vals[i]);
        }
        leaf->keys[index] = std::forward<K>(key);
        leaf->vals[index].reset(new V(std::forward<V>(val)));
        leaf->key_num++;

        if (leaf->key_num < N) {
            return;
        }
        _split_node(leaf);
    }

    void _split_node(Node* node) {
        // Node 一定满载，如果 N = 4，则有 5 个 children
        // [6,7,8,9]
        // / | | | \
        // std::cout << "split node:" << node << " " << node->dump() << " parent:" << node->parent;
        // if (node->parent) {
            // std::cout << " " << node->parent->dump();
        // };
        // std::cout << std::endl;
        // 需要分裂
        int mid = (N-1) / 2;
        auto s = new Node;
        // copy node->keys[mid+1, N) -> s->keys[0, N-mid-1)
        for (int i = mid+1; i < N; ++i) {
            s->keys[i-mid-1] = node->keys[i];
            s->vals[i-mid-1] = std::move(node->vals[i]);
            s->children[i-mid-1] = node->children[i];
            if (node->children[i]) node->children[i]->parent = s;
            node->children[i] = nullptr;
        }
        // 最后个孩子
        s->children[N-mid-1] = node->children[N];
        if (node->children[N]) node->children[N]->parent = s;
        node->children[N] = nullptr;

        // 更新 key num
        node->key_num = mid;
        s->key_num = N - mid - 1;

        // 更新指针
        auto p = node->parent;
        s->parent = p;

        // std::cout << "old node" << node->dump() << " new node:" << s << " " << s->dump() << " parent:" << s->parent << std::endl;
        // if (p) std::cout << "p:" << p;
        // if (p && p->parent) std::cout << " p->parent:" << p->parent << " " << p->parent->dump();
        // std::cout << std::endl;

        if (p == nullptr) {
            p = new Node;
            _root = p;
            node->parent = p;
            s->parent = p;
            p->keys[0] = node->keys[mid];
            p->vals[0] = std::move(node->vals[mid]);
            p->children[0] = node;
            p->children[1] = s;
            p->key_num = 1;
            // std::cout << "update root:" << p << " " << p->dump() << " parent:" << p->parent<< std::endl;
            return;
        }


        // copy node->keys[mid] -> parent->keys
        int index = _find_index(p, node->keys[mid]);
        // std::cout << "index:" << index <<  " key_num:" << p->key_num << std::endl;
        for (int i = p->key_num-1; i >= index; --i) {
            p->keys[i+1] = p->keys[i];
            p->vals[i+1] = std::move(p->vals[i]);
            p->children[i+2] = p->children[i+1];
        }
        p->keys[index] = node->keys[mid];
        p->vals[index] = std::move(node->vals[mid]);
        p->children[index+1] = s;
        p->key_num++;
        if (p->key_num < N) return;

        _split_node(p);
    }
        

    // 二分
    int _find_index(Node* node, Key key) {
        int l = 0;
        int r = node->key_num;
        while (l < r) {
            int mid = l + (r-l)/2;
            if (node->keys[mid] >= key) {
                r = mid;
            } else {
                l = mid + 1;
            }
        }
        return l;
    }

    Node* _find_leaf(Key key) {
        if (!_root) return nullptr;
        auto node = _root;
        for (;;) {
            int index = _find_index(node, key);
            if (node->children[index]) {
                node = node->children[index];
            } else {
                return node;
            }
        }
        return nullptr;
    }
private:
    Node* _root = nullptr;
};

#pragma once

#include <memory>
#include <queue>
#include <vector>
#include <optional>
#include <iostream>

#include <binary_tree.h>

template <typename K, typename V>
class BinarySearchTree : public BinaryTree<std::pair<K, V>> {
public:
    BinarySearchTree() {}
    BinarySearchTree(std::vector<std::optional<std::pair<K, V>>>&& list) :
        BinaryTree<K, V>(std::move(list)) {}

    // 递归算法查询 key
    std::optional<V> search(const K& k, int* compare_count = nullptr) {
        return {};
    }

    // 迭代算法查询
    std::optional<V> iterative_search(const K& k, int* compare_count = nullptr) {
        return {};
    }
    
    // 求最小
    std::optional<K> minimum() {
    }

    // 求最大
    std::optional<K> maximum() {
    }

    template <typename Key, typename Value>
    void insert(Key&& key, Value&& val) {
    }

    bool remove(const K& key) {
        return false;
    }
protected:
};

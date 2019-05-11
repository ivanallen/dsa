#pragma once
#include <memory>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <optional>

constexpr int SKIPLIST_MAXLEVEL = 32;

template <typename K, typename V>
class SkipList {
public:
    struct Node {
        struct Level {
            Node* forward;
            int span = 0;
        };
        std::unique_ptr<K> key;
        std::unique_ptr<V> value;
        std::vector<Level> level;
    };

    SkipList() {
        _head = new Node;
        _head->level.resize(SKIPLIST_MAXLEVEL);
        srand(time(0));
    }

    ~SkipList() {
    }

    template<typename Key, typename Value>
    void insert(Key&& key, Value&& val) {
        // 记录新插入的节点在每一层的位置
        int level = random_level();
        if (level > _level) _level = level;

        std::vector<Node*> update(_level, _head);
        std::vector<int> rank(_level, 0); // 记录节点的排位

        auto node = create_node(level, std::forward<Key>(key), std::forward<Value>(val));
        for (int k = level - 1; k >= 0; --k) {
            while (update[k]->level[k].forward && *update[k]->level[k].forward->key < key) {
                rank[k] += update[k]->level[k].span; // 记录当前 update[k] 节点的 rank 位置。
                update[k] = update[k]->level[k].forward;
            }
        }

        for (int k = level - 1; k >= 0; --k) {
            auto next = update[k]->level[k].forward;
            update[k]->level[k].forward = node;
            node->level[k].forward = next;
            node->level[k].span = next ? (update[k]->level[k].span - (rank[0]-rank[k])) : 0;
            update[k]->level[k].span = rank[0] - rank[k] + 1;
        }
    }

    std::optional<V> get(const K& key) {
        int k = _level - 1;
        auto p = _head;
        while (k >= 0) {
            auto q = p->level[k].forward;
            if (!q || key < *q->key) --k;
            else if (*q->key < key) p = q;
            else return *q->value;
        }
        return {};
    }

    std::string dump() {
        std::stringstream oss;
        for (int i = _level - 1; i >= 0; --i) {
            auto p = _head->level[i].forward;
            if (!p) continue;
            //oss << std::setfill('0');
            oss << "(-inf,-," << std::setw(2) << _head->level[i].span << ")";
            oss << placeholer(_head->level[i].span);
            while (p) {
                oss << "> (" << std::setw(2) << *p->key
                    << "," << std::setw(2) << *p->value
                    << "," << std::setw(2) << p->level[i].span << ")"
                    << placeholer(p->level[i].span);
                p = p->level[i].forward;
            }
            oss << std::endl;
        }
        return oss.str();
    }

private:
    template<typename Key, typename Value>
    Node* create_node(int level, Key&& key, Value&& val) {
        auto node = new Node;
        node->key.reset(new K(std::forward<Key>(key)));
        node->value.reset(new K(std::forward<Value>(val)));
        node->level.resize(level);
        return node;
    }

    int random_level() {
        int level = 1;
        while (rand() & 1) {
            level++;
        }
        return level < SKIPLIST_MAXLEVEL ? level : SKIPLIST_MAXLEVEL;
    }

    std::string placeholer(int span) {
        if (span == 0) return "";
        return " " + std::string(14 * (span-1) + 1, '-');
    }
private:
    int _level = 0;
    Node* _head = nullptr;
};

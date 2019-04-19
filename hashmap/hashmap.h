#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <optional>
#include <sstream>
#include <iostream>

static constexpr int HASH_TABLE_INIT_SIZE = 4;
static constexpr int FORCE_RESIZE_RATIO = 2;

template <typename K, typename V>
struct HashTable {
    struct Entry {
        Entry* next = nullptr;
        std::unique_ptr<K> key;
        std::unique_ptr<V> value;
    };

    uint32_t used = 0;
    uint32_t size = 0;
    uint32_t mask = 0;
    Entry** table = nullptr;
};

template <typename K, typename V>
class HashMap {
public:
    class Iterator {
        friend class HashMap;
    public:
        Iterator operator++(int) {
            Iterator it = *this;
            increment();
            return it;
        }
        Iterator& operator++() {
            increment();
            return *this;
        }
        void increment() {
            if (end) {
                return;
            }

            if (next) {
                current = next;
            } else {
                ++index;
                while (table < 2) {
                    end = false;

                    while (index < hashmap->_ht[table].size && hashmap->_ht[table].table[index] == nullptr) ++index;
                    if (index < hashmap->_ht[table].size) {
                        current = hashmap->_ht[table].table[index];
                        break;
                    }

                    index = 0;
                    current = nullptr;
                    next = nullptr;
                    end = true;

                    if (!hashmap->_is_rehashing()) {
                        return;
                    }
                    table++;
                }
            }

            if (end) return;

            next = current->next;
            pair = std::make_pair(*current->key, *current->value);
        }
        std::pair<K,V>* operator->() {
            return &pair;
        }
        std::pair<K,V>& operator*() {
            return pair;
        }
        bool operator==(const Iterator& rhs) const {
            if (end && rhs.end) return true;
            return table == rhs.table
                && index == rhs.index
                && current == rhs.current
                && next == rhs.next;
        }

        bool operator!=(const Iterator& rhs) const {
            return !(*this == rhs);
        }
    private:
        Iterator(HashMap* hashmap) : hashmap(hashmap) {}
    private:
        HashMap* hashmap = nullptr; 
        int table = 0;
        int index = 0;
        typename HashTable<K,V>::Entry* current = nullptr;
        typename HashTable<K,V>::Entry* next = nullptr;
        std::pair<K,V> pair;
        bool end = true;
    };
public:
    ~HashMap() {
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < _ht[i].size; ++j) {
                auto entry = _ht[i].table[j];
                while (entry) {
                    auto next = entry->next;
                    delete entry;
                    entry = next;
                }
            }
            if (_ht[i].size != 0) delete[] _ht[i].table;
        }
    }

    Iterator begin() {
        Iterator it(this);
        it.end = false;

        if (_ht[0].size == 0) {
            it.end = true;
            return it;
        }

        it.table = 0;
        it.index = 0;
        // 查找第一个不空的 entry
        while (it.index < _ht[0].size && _ht[0].table[it.index] == nullptr) ++it.index;
        auto entry = _ht[0].table[it.index];
        it.current = entry;
        it.next = entry->next;
        it.pair = std::make_pair(*entry->key, std::ref(*entry->value));
        return it;
    }

    Iterator end() {
        Iterator it(this);
        return it;
    }

    template <typename Key, typename Value>
    bool add(Key&& key, Value&& val) {
        if (_is_rehashing()) {
            _do_rehash(1);
        }

        int index = key_index(std::forward<Key>(key));
        if (index == -1) return false;

        // 如果正在 rehash，直接插入表 1
        auto& ht = _is_rehashing() ? _ht[1] : _ht[0];

        auto entry = new typename HashTable<K,V>::Entry();
        entry->next = ht.table[index];
        ht.table[index] = entry;
        ht.used++;

        entry->key.reset(new K(std::forward<Key>(key)));
        entry->value.reset(new V(std::forward<Value>(val)));
        return true;
    }

    std::optional<V> get(const K& key) const {
        if (_ht[0].size == 0) return {};
        if (_is_rehashing()) {
            _do_rehash(1);
        }

        auto h = std::hash<K>{}(key);
        for (int i = 0; i < 2; ++i) {
            auto index = h & _ht[i].mask;
            auto entry = _ht[i].table[index];
            while (entry) {
                if (*entry->key == key) {
                    return *entry->value;
                }
                entry = entry->next;
            }
            if (!_is_rehashing()) break;
        }
        return {};
    }

    // rehash 状态下返回 ht[1] 中的索引
    int key_index(const K& key) {
        auto& ht = _is_rehashing() ? _ht[1] : _ht[0];
        auto h = std::hash<K>{}(key);
        auto index = h & ht.mask;

        _expand_hash_table_if_needed();

        auto entry = ht.table[index];

        while (entry) {
            if (*entry->key == key) {
                return -1;
            }
            entry = entry->next;
        }

        return index;
    }

    // 返回当前状态
    std::string stat() {
        std::stringstream oss;
        oss << "================ statistic ================" << std::endl;
        oss << "rehashing:" << _is_rehashing() << std::endl;
        oss << "ht[0]" << std::endl;
        oss << "size:" << _ht[0].size << std::endl;
        oss << "used:" << _ht[0].used << std::endl;

        oss << "ht[1]" << std::endl;
        oss << "size:" << _ht[1].size << std::endl;
        oss << "used:" << _ht[1].used << std::endl;
        oss << "================ statistic ================";

        return oss.str();
    }

private:
    bool _is_rehashing() const {
        return _rehash_index != -1;
    }

    int _do_rehash(int n) const {
        if (!_is_rehashing()) return 0; // 表示已经完成

        while (n--) {
            if (_ht[0].used == 0) {
                delete[] _ht[0].table; // 不可能为空
                _ht[0] = _ht[1];
                _ht[1].used = 0;
                _ht[1].size = 0;
                _ht[1].table = nullptr;
                _rehash_index = -1;
                return 1;
            }
            assert(_rehash_index < _ht[0].size);
            while (_ht[0].table[_rehash_index] == nullptr) _rehash_index++;

            auto entry = _ht[0].table[_rehash_index];

            while (entry) {
                auto next = entry->next;
                auto index = std::hash<K>{}(*entry->key) & _ht[1].mask;
                entry->next = _ht[1].table[index];
                _ht[1].table[index] = entry;
                _ht[0].used--;
                _ht[1].used++;
                entry = next;
            }

            _ht[0].table[_rehash_index] = nullptr;
            _rehash_index++;
        }
        return 0;
    }

    void _expand_hash_table_if_needed() {
        if (_is_rehashing()) return;

        if (_ht[0].size == 0) {
            return _expand_hash_table(HASH_TABLE_INIT_SIZE);
        }

        // debug
        // std::cout << "load factor:" << _ht[0].used * 1.0 / _ht[0].size << std::endl;
        if (_ht[0].used > _ht[0].size && _ht[0].used * 1.0 / _ht[0].size > FORCE_RESIZE_RATIO) {
            return _expand_hash_table(_ht[0].used * 2);
        }
    }

    void _expand_hash_table(int size) {
        if (_is_rehashing()) return;

        if (_ht[0].table == nullptr) {
            _ht[0].table = new typename HashTable<K,V>::Entry*[HASH_TABLE_INIT_SIZE]();
            _ht[0].size = HASH_TABLE_INIT_SIZE;
            _ht[0].mask = HASH_TABLE_INIT_SIZE - 1;
            return;
        }

        uint32_t expand_size = 1;

        while (expand_size < size) {
            expand_size <<= 1;
        }

        _ht[1].table = new typename HashTable<K,V>::Entry*[expand_size]();
        _ht[1].size = expand_size;
        _ht[1].mask = expand_size - 1;
        _rehash_index = 0; // 开启 rehash
        return;
    }
private:
    mutable HashTable<K, V> _ht[2];
    mutable int _rehash_index = -1;
};

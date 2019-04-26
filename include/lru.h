#pragma once
#include <list/list.h>
#include <map>
#include <iostream>
#include <optional>

template <typename K, typename V, unsigned int N>
class LRU {
public:
    template <typename Value>
    void insert(const K& key, Value&& val) {
        auto it = _m.find(key);
        if (it != _m.end()) {
            it->second.move_to(_list.begin());
            it->second->second = std::forward<Value>(val);
            //_list.push_front(std::make_pair(std::forward<Key>(key), std::forward<Value>(val)));
            return;
        }

        _list.push_front(std::make_pair(key, std::forward<Value>(val)));
        _m.emplace(key, _list.begin());

        if (_list.size() > N) {
            _m.erase(_list.back().first);
            _list.pop_back();
        }
#ifdef _DEBUG
        std::cout << "insert:{" << key << "," << val << "} ";
        show();
#endif
    }


    std::optional<V> get(const K& key) {
        auto it = _m.find(key);
        if (it == _m.end()) return {};
        int val = it->second->second;
        it->second.move_to(_list.begin());
        return val;
    }

    void show() {
        std::cout << "[";
        _list.for_each([](const std::pair<K,V>& e) { 
            std::cout << " {" << e.first << "," << e.second << "}";
        });
        std::cout << " ]" << std::endl;
    }

private:
    List<std::pair<K,V>> _list;
    std::map<K, typename List<std::pair<K,V>>::Iterator> _m;
};

#pragma once
#include <list>
#include <map>
#include <iostream>
#include <optional>

template <typename K, typename V, unsigned int N>
class LRU {
public:
    void insert(const K& key, const V& val) {
        auto it = _m.find(key);
        if (it != _m.end()) {
            _list.erase(it->second);
            _m.erase(it);
        }

        _list.push_front(std::make_pair(key, val));
        _m[key] = _list.begin();

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
        return it->second->second;
    }

    void show() {
        std::cout << "[";
        for (auto& e : _list) {
            std::cout << " {" << e.first << "," << e.second << "}";
        }
        std::cout << " ]" << std::endl;
    }

private:
    std::list<std::pair<K,V>> _list;
    std::map<K, typename std::list<std::pair<K,V>>::iterator> _m;
};

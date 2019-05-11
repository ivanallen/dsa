#pragma once

#include <vector>
#include <iostream>
#include <functional>

template <typename T, typename Compare = std::less<T>>
class Heap {
public:
    void sink(int i) {
        // left = 2i+1, right = 2i+2
        int size = _tree.size();
        while(i < size) {
            int j = 2*i + 1;
            if (j >= size) return;

            if (j < size - 1 && _cmp(_tree[j], _tree[j+1])) ++j;
            if (_cmp(_tree[i], _tree[j]))
                std::swap(_tree[i], _tree[j]);
            i = j;
        }
    }

    template <typename U>
    void insert(U&& val) {
        _tree.push_back(std::forward<U>(val));
        for (int i = _tree.size() - 1; i >= 0; --i) {
            sink(i);
        }
    }

    void show() {
        std::cout << "[";
        for (size_t i = 0; i < _tree.size(); ++i) {
            std::cout << ' ' << _tree[i];
        }
        std::cout << " ]" << std::endl;
    }

    bool empty() {
        return _tree.empty();
    }

    T& top() {
        return _tree[0];
    }

    void pop() {
        _tree[0] = _tree.back();
        _tree.resize(_tree.size() - 1);
        sink(0);
    }
private:
    Compare _cmp;
    std::vector<T> _tree;
};

/*
 * Copyright (c) 2019 DSA authors.
 * All rights reserved.
 * Authors: Allen
 */

// 动态数组
//

#pragma once

#include <new>
#include <type_traits>
#include <string.h>

namespace dsa {

template <typename T>
class Vector {
public:
    typedef T* Iterator;

    Vector() {}

    Vector(size_t n) {
        _start = new Storage[n];
        _finish = _start;
        _end_of_storage = _start + n;
    }

    // TODO:
    Vector(int size, const T& a);

    // TODO:
    Vector(const Vector<T>& a);

    // TODO:
    Vector(Iterator first, Iterator last);

    ~Vector() {
        if (_start == nullptr) return;
        for (auto it = _start; it != _finish; ++it) {
            std::launder(reinterpret_cast<T*>(it))->~T();
        }
        delete[] _start;
    }

    size_t size() const {
        return _finish - _start;
    }

    size_t capacity() const {
        return _end_of_storage - _start;
    }

    // about std::launder: https://stackoverflow.com/questions/39382501/what-is-the-purpose-of-stdlaunder
    // https://miyuki.github.io/2016/10/21/std-launder.html
    template <typename U>
    void push_back(U&& val) {
        if (_finish == _end_of_storage) {
            size_t old_size = size();
            size_t len = old_size == 0 ? 1 : 2 * old_size;
            auto new_start = new Storage[len];
            if (_start) {
                uninitialized_copy((const char*)_start, (const char*)_finish, (char*)new_start);
                for (auto it = _start; it != _finish; ++it) {
                    std::launder(reinterpret_cast<T*>(it))->~T();
                }
                delete[] _start;
                _start = new_start;
                _finish = _start + old_size;
                _end_of_storage = _start + len;
            } else {
                _start = new_start;
                _finish = _start;
                _end_of_storage = _start + len;
            }
        }
        new (_finish) T(std::forward<U>(val));
        ++_finish;
    }

    // TODO:
    void pop_back();

    const T& operator[](int index) const {
        // TODO: throw exception if index invalid
        return *std::launder(reinterpret_cast<const T*>(&_start[index]));
    }

    T& operator[](int index) {
        // TODO: throw exception if index invalid
        return *std::launder(reinterpret_cast<T*>(&_start[index]));
    }

    // TODO:
    Vector<T>& operator=(const Vector<T>& a);
    Vector<T>& operator=(Vector<T>&& a);

    bool empty() const {
        return _start == _finish;
    }

    Iterator begin() const {
        return std::launder(reinterpret_cast<T*>(_start));
    }
    Iterator end() const {
        return std::launder(reinterpret_cast<T*>(_finish));
    }
private:
    char* uninitialized_copy(const char* first, const char* last, char* result) {
        memmove(result, first, last - first);
        return result + (last - first);
    }
private:
    using Storage = typename std::aligned_storage<sizeof(T), alignof(T)>::type;
    Storage* _start = nullptr;
    Storage* _finish = nullptr;
    Storage* _end_of_storage = nullptr;
};

} // namespace dsa

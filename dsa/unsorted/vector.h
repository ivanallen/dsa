#pragma once
#include<cassert>
typedef unsigned int  size_t;
template < class T>
class Vector {
public:
    typedef T* iterator;
    
    Vector();
    Vector(int size, T const& a);
    Vector(const Vector<T> & a);
    Vector(iterator first, iterator last);
    ~Vector();
    size_t size() const;
    size_t capacity() const;
    size_t max_capacity() const;
    void push_back(const T& val);
    void pop_back();
    T& operator[](int index);
    Vector<T>& operator=(const Vector<T>& a);
    bool empty() const;
    iterator begin() const;
    iterator end() const;
private:
    size_t  _size;
    size_t  _capacity;
    T*      _buf;
    const size_t _max_capacity = 65536;
};
template<class T>
Vector<T>::Vector()
{   
    _size = 0;
    _buf = new T[1];
    _capacity = 1;
}

template<class T>
Vector<T>::Vector(int s, const T& a)
{   
    if (s > _max_capacity) {
        s = _max_capacity;
    }
    _size = s;
    _capacity = 1;
    while (_capacity < _size) {
        _capacity *= 2;
    }
    _buf = new T[_capacity];
    for (size_t i = 0; i < _size; i++) {
        _buf[i] = a;
    }
}
template<class T>
Vector<T>::Vector(const Vector<T> & a)
{
    _size = a._size;
    _capacity = a._capacity;
    _buf = new T[_capacity];
    for (size_t i = 0; i < _size; i++) {
        _buf[i] = a._buf[i];
    }
}

template<class T>
Vector<T>::Vector(iterator first, iterator last)
{
    _size = last - first;
    if (_size %2 == 0) {
        _capacity = _size;
    } else {
        _capacity = (_size / 2) * 4; 
    }
    _buf = new T[_capacity];
    int i = 0;
    for (iterator it = first; it < last; it++) {
        _buf[i] = *it;
        i++;
    }

}

template<class T>
Vector<T>::~Vector()
{
    delete[] _buf;
}

template<class T>
size_t Vector<T>::size() const
{
    return _size;
}

template<class T>
size_t Vector<T>::capacity() const
{
    return _capacity;
}
template<class T>
size_t Vector<T>::max_capacity() const
{
    return _max_capacity;
}
template<class T>
T& Vector<T>::operator[](int index)
{
    assert(index >= 0 && index < _size);
    return _buf[index];
}

template<class T>
void Vector<T>::push_back(const T& val)
{
    if (_size < _capacity) {
        _buf[_size] = val;
        _size++;
        return ;
    } else if (_size == _max_capacity) {
        return ;
    }
    _capacity *= 2;
    if (_capacity >= _max_capacity) {
        _capacity = _max_capacity;
    }
    T * tmp = new T[_capacity];
    for (size_t i = 0; i < _size; i++) {
        tmp[i] = _buf[i];
    }
    tmp[_size] = val;
    _size++; 
    delete[] _buf;
    _buf = tmp; 
}

template<class T>
void Vector<T>::pop_back()
{
    assert(_size > 0);
    _size--;
}
template<class T>
bool Vector<T>::empty() const
{
    if (_size == 0) {
        return true;
    }
    return false;
}
template<class T>
typename Vector<T>::iterator Vector<T>::begin() const
{
    return _buf;
}
template<class T>
typename Vector<T>::iterator Vector<T>::end() const
{
    return _buf + _size;
}
template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T> & a)
{
    if (this == &a) {
        return *this ;
    }
    delete[] _buf;
    _size = a._size;
    _capacity = a._capacity;
    _buf = new T[_capacity];
    for (size_t i = 0; i < _size; i++) {
        _buf[i] = a._buf[i];
    }
    return *this;
}


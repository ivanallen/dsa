#pragma
#include <sstream>
#include <memory>
#include <stdexcept>

// 双向链表
// head <=> @ <=> @ <=> @ <=> tail
// 其中 head 和 tail 是 guard，不保存实际数据
template <typename T>
class List {
public:
    class Iterator;
    struct Entry {
        std::unique_ptr<T> value;
        Entry* prev = nullptr;
        Entry* next = nullptr;
    };

    List() {
        _head.next = &_tail;
        _tail.prev = &_head;
    }

    ~List() {
        auto next = begin();
        while(next != end()) {
            next = erase(next);
        }
    }

    bool empty() {
        return _head.next == &_tail;
    }

    Iterator erase(const Iterator& it) {
        auto prev = it.current->prev;
        auto next = it.current->next;

        prev->next = next;
        next->prev = prev;
        delete it.current;

        Iterator ret(this);
        ret.current = next;
        --_length;
        return ret;
    }
    
    Iterator begin() {
        Iterator it(this);
        it.current = _head.next;
        return it;
    }

    Iterator end() {
        Iterator it(this);
        it.current = &_tail;
        return it;
    }

    Iterator rbegin() {
        Iterator it(this);
        it.current = _tail.prev;
        return it;
    }

    Iterator rend() {
        Iterator it(this);
        it.current = &_head;
        return it;
    }

    T& back() {
        return *_tail.prev->value;
    }

    void pop_back() {
        erase(rbegin());
    }

    T& front() {
        return *_head.next;
    }

    template <typename U>
    void push_back(U&& val) {
        auto s = new Entry;
        s->value.reset(new T(std::forward<U>(val)));
        if (_length == 0) {
            _head.next = s;
            _tail.prev = s;
            s->prev = &_head;
            s->next = &_tail;
        } else {
            auto last = _tail.prev;    
            last->next = s;
            s->prev = last;
            s->next = &_tail;
            _tail.prev = s;
        }
        ++_length;
    }

    template <typename U>
    void push_front(U&& val) {
        auto s = new Entry;
        s->value.reset(new T(std::forward<U>(val)));
        if (_length == 0) {
            _head.next = s;
            _tail.prev = s;
            s->prev = &_head;
            s->next = &_tail;
        } else {
            auto first = _head.next;    
            _head.next = s;
            first->prev = s;
            s->next = first;
            s->prev = &_head;
        }
        ++_length;
    }

    template <typename FUNC>
    void for_each(FUNC&& cb) const {
        auto p = _head.next;
        while (p != &_tail) {
            cb(*p->value);
            p = p->next;
        }
    }

    Iterator find(const T& val) {
        auto it = begin();
        while (it != end()) {
            if (*it == val) {
                return it;
            }
            ++it;
        }
        return it;
    }

    int length() const {
        return _length;
    }

    int size() const {
        return _length;
    }

    std::string dump(bool reverse=false) {
        std::stringstream oss;
        oss << "[";
        if (!reverse) {
            auto p = _head.next;
            if (p != &_tail) {
                oss << *p->value;
                p = p->next;
            }
            while (p != &_tail) {
                oss << "," << *p->value;
                p = p->next;
            }
        } else {
            auto p = _tail.prev;
            if (p != &_head) {
                oss << *p->value;
                p = p->prev;
            }
            while (p != &_head) {
                oss << "," << *p->value;
                p = p->prev;
            }
        }
        oss << "]";
        return oss.str();
    }

private:
    Entry _head;
    Entry _tail;
    int _length;
};

template <typename T>
class List<T>::Iterator {
public:
    friend class List;

    // 后置++
    Iterator operator++(int) {
        Iterator it = *this;
        increment();
        return it;
    }

    Iterator& operator++() {
        increment();
        return *this;
    }

    // 后置--
    Iterator operator--(int) {
        Iterator it = *this;
        decrement();
        return it;
    }

    Iterator& operator--() {
        decrement();
        return *this;
    }

    bool operator==(const Iterator& rhs) const {
        return current == rhs.current;
    }

    bool operator!=(const Iterator& rhs) const {
        return current != rhs.current;
    }

    T& operator*() {
        return *current->value;
    }

    T* operator->() {
        return current->value.get();
    }

    // 将某个节点移动到指定位置
    bool move_to(const Iterator& pos) {
        if (pos.list != list) {
            return false;
        }
        // detach
        auto prev = current->prev;
        auto next = current->next;

        prev->next = next;
        next->prev = prev;

        // insert
        auto after = pos.current;
        auto before = after->prev;
        before->next = current;
        current->prev = before;
        current->next = after;
        after->prev = current;
        return true;
    }

    void increment() {
        if (reverse) {
            decrement();
            return;
        }

        if (current == &list->_tail) {
            return;
        }
        current = current->next;
    }

    void decrement() {
        if (reverse) {
            increment();
            return;
        }

        if (current == &list->_head) {
            return;
        }
        current = current->prev;
    }
private:
    Iterator(List *list, bool reverse = false) : list(list), reverse(reverse) {}
    Entry *current = nullptr;
    List *list = nullptr;
    bool reverse = false;
};

#include <deque>

// 默认单调递增
template <typename T, typename Compare = std::less<T>>
class MonoQueue {
public:
    MonoQueue() : _cmp(Compare()) {}

    template <typename U>
    void push(U&& e) {
        while (!_q.empty() && _cmp(e, _q.back())) {
            _q.pop_back();
        }
        _q.push_back(std::forward<U>(e));
    }

    void pop() {
        _q.pop_front();
    }

    T& front() {
        return _q.front();
    }

    const T& front() const {
        return _q.front();
    }

    bool empty() const {
        return _q.empty();
    }
private:
    std::deque<T> _q;
    Compare _cmp;
};

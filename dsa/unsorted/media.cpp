#include <queue>
#include <vector>
#include <iostream>
#include <stdlib.h>

class Media {
public:
    void insert(int v) {
        if (_max_heap.empty()) {
            _max_heap.push(v);
            return;
        }

        if (_min_heap.empty()) {
            _min_heap.push(v);
            return;
        }

        int up = _min_heap.top();
        int down = _max_heap.top();

        if (v < down) _max_heap.push(v);
        else if (v > up) _min_heap.push(v);
        else {
            if (_max_heap.size() <= _min_heap.size()) {
                _max_heap.push(v);
            } else {
                _min_heap.push(v);
            }
        }

        if (_max_heap.size() < _min_heap.size()) {
            _max_heap.push(_min_heap.top());
            _min_heap.pop();
        }
        if (_min_heap.size() + 1 < _max_heap.size()) {
            _min_heap.push(_max_heap.top());
            _max_heap.pop();
        }
    };

    int media() {
        return _max_heap.top();
    }
private:
    std::priority_queue<int> _max_heap;
    std::priority_queue<int, std::vector<int>, std::greater<int>> _min_heap;
};

int main() {
    Media m;
    srand(time(0));
    std::vector<int> v;
    for (int i = 0; i < 10; ++i) {
        int r = rand() % 100;
        v.push_back(r);
        std::cout << r << " ";
    }

    std::cout << std::endl;


    for (auto e : v) {
        m.insert(e);
        std::cout << m.media() << ' ';
    }
    std::cout << std::endl;
    return 0;
}

#include <lru/lru.h>

int main() {
    LRU<int, int, 5> lru;

    for (int i = 0; i < 5; ++i) {
        lru.insert(i, i);
    }

    lru.insert(2, 2);
    lru.insert(5, 5);
    lru.insert(8, 100);

    lru.show();

    std::cout << "get(8):" << lru.get(8).value_or(-1) << std::endl;
    std::cout << "get(9):" << lru.get(100).value_or(-1) << std::endl;

    return 0;
}

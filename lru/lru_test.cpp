#include <lru/lru.h>

int main() {
    LRU<int, int, 5> lru;

    for (int i = 0; i < 5; ++i) {
        lru.insert(i, i);
    }

    lru.show();
    std::cout << "insert(2, 2)" << std::endl;
    lru.insert(2, 2);
    lru.show();
    std::cout << "insert(5, 5)" << std::endl;
    lru.insert(5, 5);
    lru.show();
    std::cout << "insert(8, 10)" << std::endl;
    lru.insert(8, 100);
    lru.show();
    std::cout << "get(3):" << lru.get(3).value_or(-1) << std::endl;
    lru.show();
    std::cout << "get(9):" << lru.get(9).value_or(-1) << std::endl;
    lru.show();

    return 0;
}

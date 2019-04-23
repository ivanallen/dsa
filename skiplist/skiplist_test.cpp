#include <skiplist.h>
#include <iostream>

int main() {
    SkipList<int, int> list;

    for (int i = 0; i < 15; ++i) {
        list.insert(i, 2*i+1);
    }

    std::cout << list.dump() << std::endl;

    std::cout << "get(3) = " << list.get(3).value_or(-1) << std::endl;
    std::cout << "get(20) = " << list.get(20).value_or(-1) << std::endl;
    return 0;
}

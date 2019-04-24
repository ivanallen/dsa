#include <iostream>
#include <list.h>

int main() {
    List<int> list;
    for (int i = 0; i < 10; ++i) {
        if (i&1) {
            list.push_back(i);
        } else {
            list.push_front(i);
        }
    }
    std::cout << list.dump() << std::endl;
    std::cout << list.dump(true) << std::endl;

    list.for_each([](int v) {
        std::cout << v;
    });
    std::cout << std::endl;

    auto it = list.begin();
    while (it != list.end()) {
        std::cout << *it;
        ++it;
    }
    std::cout << std::endl;


    it = list.find(0);
    std::cout << "find 0:" << *it << std::endl;
    if (it != list.end()) {
        it.move_to(list.begin());
    }

    std::cout << list.dump() << std::endl;
    return 0;
}

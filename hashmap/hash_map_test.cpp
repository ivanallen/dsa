#include <string>
#include <iostream>
#include <hashmap.h>

int main() {
    HashMap<int, std::string> h;
    std::cout << std::boolalpha;
    std::cout << "add(10, hello):" << h.add(10, "hello") << std::endl;
    std::cout << "add(11, world):" << h.add(11, "world") << std::endl;

    std::cout << h.get(10).value_or("none") << std::endl;
    std::cout << h.get(11).value_or("none") << std::endl;
    std::cout << h.get(1).value_or("none") << std::endl;

    std::cout << std::endl;
    std::cout << h.stat() << std::endl;

    for (int i = 0; i < 30; ++i) {
        std::cout << "add(" << i << ", world):" << h.add(i, "world") << std::endl;
        std::cout << h.get(i).value_or("none") << std::endl;
    }
    std::cout << std::endl;
    std::cout << h.stat() << std::endl;

    HashMap<int, std::string>::Iterator it = h.begin();
    while (it != h.end()) {
        std::cout << "(" << it->first << "," << it->second << ") => ";
        it->second = "hello, world";
        std::cout << "(" << it->first << "," << it->second << ")" << std::endl;
        ++it;
    }
    return 0;
}

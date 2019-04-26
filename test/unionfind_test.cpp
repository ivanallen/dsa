#include <iostream>
#include "../include/unionfind.h"

void test(UnionFind& uf, int a, int b) {
    std::cout << "connect(" << a << ',' << b << ") = " << uf.connect(a,b) << std::endl;
    std::cout << "connected(" << a << ',' << b << ") = " << uf.connected(a,b) << std::endl;
    std::cout << "count() = " << uf.count() << std::endl;
}

int main() {
    UnionFind uf(10);
    test(uf, 1, 2);
    test(uf, 2, 3);
    test(uf, 1, 3);
    return 0;
}

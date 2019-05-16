#include <iostream>
#include "vector.h"

using namespace dsa;

class Foo {
    friend std::ostream& operator<<(std::ostream& out, const Foo& foo);
public:
    Foo(int n, int data) : n(n), data(data) {
        std::cout << "Foo():" << data << std::endl;
    };
    ~Foo() {
        std::cout << "~Foo():" << data << std::endl;
    }

    Foo(Foo&& foo) : n(foo.n) {
        std::cout << "Foo(Foo&&)" << std::endl;
        data = foo.data;
    }
    const int n;
    int data = 0;
};

std::ostream& operator<<(std::ostream& out, const Foo& foo) {
    return out << foo.data;
}

int main() {
    Vector<Foo> v(50);

    for (int i = 0; i < 10; ++i) {
        v.push_back(Foo(i, i*2));
    }

    for (auto& e : v) {
        std::cout << e << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < v.size(); ++i) {
        std::cout << v[i].n << " ";
    }
    std::cout << std::endl;

    Vector<int> v2;

    for (int i = 0; i < 10; ++i) {
        v2.push_back(i);
    }

    for (int i = 0; i < v2.size(); ++i) {
        std::cout << v2[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}

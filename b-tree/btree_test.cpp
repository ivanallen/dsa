#include <btree.h>

class Foo {
public:
    friend std::ostream& operator<<(std::ostream& o, const Foo& f);
    Foo(int n) : _n(n), _s(std::to_string(n)) {}
    Foo(const Foo& rhs) {
        _n = rhs._n;
        std::cout << "Foo(const Foo&):" << _s << std::endl;
    }
    Foo(Foo&& rhs) {
        _n = rhs._n;
        _s = std::move(rhs._s);
        std::cout << "Foo(Foo&&):" << _s << std::endl;
    }
    ~Foo() {
        std::cout << "~Foo():" << _s << std::endl;
    }
private:
    int _n;
    std::string _s;
};

std::ostream& operator<<(std::ostream& o, const Foo& f) {
    return o << f._n;
}

int main() {
    BTree<int, Foo, 8> bt;
    for (int i = 0; i < 18; ++i) {
        bt.insert(i, Foo(i));
    }
    for (int i = 5; i < 7; ++i) {
        bt.insert(i, Foo(i+i));
    }
    bt.print();

    for (int i = 0; i < 20; ++i) {
        auto v = bt.find(i);
        if (v) {
            std::cout << '(' << i << ',' << *v << ')' << std::endl;
        } else {
            std::cout << '(' << i << ",?)" << std::endl;
        }
    }
}

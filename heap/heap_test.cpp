#include <heap/heap.h>

int main() {
    // Heap<int> hp; // 大顶堆
    Heap<int, std::greater<int>> hp; // 小顶堆
    hp.insert(5);
    hp.insert(4);
    hp.insert(9);
    hp.insert(0);
    hp.insert(2);
    hp.insert(8);
    hp.insert(7);
    hp.show();

    std::cout << "pop: ";
    while(!hp.empty()) {
        std::cout << " " << hp.top();
        hp.pop();
    }
    std::cout << std::endl;
    return 0;
}

#include <vector>
#include <iostream>
#include <sort/quick_sort.h>

int main() {
    std::vector<int> a = {5, 9, 1, 2, 3, 4, 6, 7, 8, 1};
    quick_sort(a);

    for (auto e : a) {
        std::cout << " " << e;
    }
    std::cout << std::endl;
    return 0;
}
